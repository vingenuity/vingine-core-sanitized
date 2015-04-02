#include "DialogInterface.hpp"

#include "PlatformSpecificHeaders.hpp"

#include "DebuggerInterface.hpp"

#if defined( PLATFORM_PS3 )
#include <sysutil/sysutil_msgdialog.h>
#include <PSGL/psgl.h>

//-----------------------------------------------------------------------------------------------
void HandleDialogBoxResult( int buttonType, void* userData )
{
	int* out_buttonResult = static_cast< int* >( userData );
	*out_buttonResult = buttonType;
}
#elif defined( PLATFORM_VITA )
#include <message_dialog.h>
#include "main_vita.cpp"
#pragma comment( lib, "libSceCommonDialog_stub.a" )

void SwapBuffers(); // This is defined in main_vita.cpp
#endif


//-----------------------------------------------------------------------------------------------
Dialog::ButtonID PopUpSystemDialog( const std::string& titleText, const std::string& messageText, 
							Dialog::ButtonSet buttonSet, Dialog::Icon icon )
{
#if defined( PLATFORM_WINDOWS )
	return MessageBoxA( NULL, messageText.c_str(), titleText.c_str(), buttonSet | icon );



#elif defined( PLATFORM_HTML5 )
	if( buttonSet == Dialog::BUTTONSET_OK )
	{
		EM_ASM_ARGS( {var result = confirm( Pointer_stringify( $0 ) ); return result;}, messageText.c_str() );
		return Dialog::ID_OK;
	}
	int confirmBoxResult = EM_ASM_INT( {var result = confirm( Pointer_stringify( $0 ) ); return result;}, messageText.c_str() );

	switch( buttonSet )
	{
	case Dialog::BUTTONSET_OKOrCancel:
		{
			if( confirmBoxResult == 1 )
				return Dialog::ID_OK;
			else
				return Dialog::ID_Cancel;
		}
	case Dialog::BUTTONSET_YesOrNo:
		{
			if( confirmBoxResult == 1 )
				return Dialog::ID_Yes;
			else
				return Dialog::ID_No;
		}
	default:
		return Dialog::ID_Cancel;
	}



#elif defined( PLATFORM_PS3 )
	VARIABLE_IS_UNUSED( titleText );

	// Start with a message box that will show the background
	unsigned int messageBoxType = CELL_MSGDIALOG_TYPE_BG_VISIBLE;

	// Then, configure it from the options.
	switch( buttonSet )
	{
	case Dialog::BUTTONSET_OK:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK;
		messageBoxType |= CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON;
		break;
	case Dialog::BUTTONSET_OKOrCancel:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK;
		messageBoxType |= CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_OFF;
		break;
	case Dialog::BUTTONSET_YesOrNo:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO;
		messageBoxType |= CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON;
		break;
	case Dialog::BUTTONSET_RetryOrCancel:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO;
		messageBoxType |= CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON;
		break;
	case Dialog::BUTTONSET_CancelTryAgainOrContinue:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO;
		messageBoxType |= CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_OFF;
		break;
	default:
		messageBoxType |= CELL_MSGDIALOG_TYPE_BUTTON_TYPE_NONE;
		break;
	}
	switch( icon )
	{
	case Dialog::ICON_Error:
	case Dialog::ICON_Warning:
		messageBoxType |= CELL_MSGDIALOG_TYPE_SE_TYPE_ERROR;
		messageBoxType |= CELL_MSGDIALOG_TYPE_SE_MUTE_OFF;
		break;
	case Dialog::ICON_Information:
	case Dialog::ICON_Question:
	case Dialog::ICON_None:
		messageBoxType |= CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL;
		messageBoxType |= CELL_MSGDIALOG_TYPE_SE_MUTE_OFF;
		break;
	}

	int buttonPressed = CELL_MSGDIALOG_BUTTON_NONE;
	static void* PARAMETER_UNUSED_IN_CURRENT_SDK = nullptr;
	cellMsgDialogOpen2( messageBoxType, messageText.c_str(), HandleDialogBoxResult, (void*)&buttonPressed, PARAMETER_UNUSED_IN_CURRENT_SDK );

	//Wait for the result while refreshing the screen so that users can see the box.
	while( buttonPressed == CELL_MSGDIALOG_BUTTON_NONE )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		cellSysutilCheckCallback();
		psglSwap();
	}

	Dialog::ButtonID buttonReturned = Dialog::ID_Cancel;
	switch( buttonPressed )
	{
	case CELL_MSGDIALOG_BUTTON_OK: /* also equivalent to CELL_MSGDIALOG_BUTTON_YES */
		{
			if( buttonSet == Dialog::BUTTONSET_CancelTryAgainOrContinue )
				buttonReturned = Dialog::ID_Continue;
			else if( buttonSet == Dialog::BUTTONSET_RetryOrCancel )
				buttonReturned = Dialog::ID_Retry;
			else if( buttonSet == Dialog::BUTTONSET_YesOrNo )
				buttonReturned = Dialog::ID_Yes;
			else
				buttonReturned = Dialog::ID_OK;
			break;
		}
	case CELL_MSGDIALOG_BUTTON_NO:
		{
			if( buttonSet == Dialog::BUTTONSET_CancelTryAgainOrContinue )
				buttonReturned = Dialog::ID_TryAgain;
			else if( buttonSet == Dialog::BUTTONSET_RetryOrCancel )
				buttonReturned = Dialog::ID_Cancel;
			else if( buttonSet == Dialog::BUTTONSET_YesOrNo )
				buttonReturned = Dialog::ID_No;
			else
				buttonReturned = Dialog::ID_Cancel;
			break;
		}
	case CELL_MSGDIALOG_BUTTON_ESCAPE:
	case CELL_MSGDIALOG_BUTTON_INVALID:
	case CELL_MSGDIALOG_BUTTON_NONE:
	default:
		break;
	}

	cellMsgDialogClose( 0 ); // If we don't close the popup, the buttons won't disappear.
	return buttonReturned;

#elif defined( PLATFORM_VITA )
	int opResult = SCE_OK;

	// initalize parameter of message dialog
	SceMsgDialogParam				msgParam;
	sceMsgDialogParamInit(&msgParam);
	msgParam.mode = SCE_MSG_DIALOG_MODE_USER_MSG;

	// initalize parameters for infobar
	SceCommonDialogInfobarParam	infobarParam;
	memset(&infobarParam, 0, sizeof(infobarParam));
	infobarParam.visibility = true;
	infobarParam.color = 0;
	infobarParam.transparency = 0;
	msgParam.commonParam.infobarParam = &infobarParam;

	// initalize parameters for dimmer color
	SceCommonDialogColor		dimmerColor;
	dimmerColor.r = 0;
	dimmerColor.g = 0;
	dimmerColor.b = 0;
	dimmerColor.a = 255;
	msgParam.commonParam.dimmerColor = &dimmerColor;
	msgParam.commonParam.bgColor = NULL;

	// initalize message dialog
	SceMsgDialogUserMessageParam	userMsgParam;
	memset(&userMsgParam, 0, sizeof(SceMsgDialogUserMessageParam));
	msgParam.userMsgParam = &userMsgParam;
	msgParam.userMsgParam->msg = (const SceChar8*)messageText.c_str();


	// Then, configure it from the options.
	switch( buttonSet )
	{
	case Dialog::BUTTONSET_OK:
		msgParam.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
		msgParam.userMsgParam->buttonParam = nullptr;
		break;
	case Dialog::BUTTONSET_YesOrNo:
		msgParam.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_YESNO;
		msgParam.userMsgParam->buttonParam = nullptr;
		break;
	case Dialog::BUTTONSET_OKOrCancel:
	case Dialog::BUTTONSET_RetryOrCancel:
		msgParam.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK_CANCEL;
		msgParam.userMsgParam->buttonParam = nullptr;
		break;
	case Dialog::BUTTONSET_CancelTryAgainOrContinue:
		{
			msgParam.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_3BUTTONS;

			//Set strings in 3buttons. 
			SceMsgDialogButtonsParam buttonParam;
			memset(&buttonParam, 0, sizeof(SceMsgDialogButtonsParam));
			buttonParam.msg1 = "Continue";
			buttonParam.msg2 = "Try Again";
			buttonParam.msg3 = "Cancel";
			buttonParam.fontSize1 = SCE_MSG_DIALOG_FONT_SIZE_DEFAULT;
			buttonParam.fontSize2 = SCE_MSG_DIALOG_FONT_SIZE_DEFAULT;
			buttonParam.fontSize3 = SCE_MSG_DIALOG_FONT_SIZE_DEFAULT;
			msgParam.userMsgParam->buttonParam = &buttonParam;
		}
		break;
	default:
		msgParam.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_NONE;
		msgParam.userMsgParam->buttonParam = nullptr;
		break;
	}

	opResult = sceMsgDialogInit(&msgParam);
	if (opResult != SCE_OK) { 
		return Dialog::ID_Cancel;
	}

	SceCommonDialogStatus dialogStatus = SCE_COMMON_DIALOG_STATUS_NONE;
	//Wait for the result while refreshing the screen so that users can see the box.
	while( dialogStatus != SCE_COMMON_DIALOG_STATUS_FINISHED )
	{
		dialogStatus = sceMsgDialogGetStatus();

		SceCommonDialogUpdateParam	updateParam;
		memset(&updateParam, 0, sizeof(updateParam));
		updateParam.renderTarget.colorFormat    = SCE_GXM_COLOR_FORMAT_A8B8G8R8;
		updateParam.renderTarget.surfaceType    = SCE_GXM_COLOR_SURFACE_LINEAR;
		updateParam.renderTarget.width          = 960;
		updateParam.renderTarget.height         = 544;
		updateParam.renderTarget.strideInPixels = 1024;

		updateParam.renderTarget.colorSurfaceData = g_backBuffers[ g_backBufferIndex ].colorData;
		updateParam.renderTarget.depthSurfaceData = g_backBuffers[ g_backBufferIndex ].depthData;
		updateParam.displaySyncObject = g_backBuffers[ g_backBufferIndex ].syncObject;
		sceCommonDialogUpdate(&updateParam);

		SwapBuffers();
	}

	SceMsgDialogResult messageResult;
	memset( &messageResult, 0, sizeof( SceMsgDialogResult ) );
	opResult = sceMsgDialogGetResult( &messageResult );
	if ( opResult != SCE_OK ) {
		return Dialog::ID_Cancel;
	}

	// terminate message dialog
	opResult = sceMsgDialogTerm();
	if ( opResult != SCE_OK ) {
		return Dialog::ID_Cancel;
	}

	// If this happens, either we aborted the dialog or the user pressed cancel.
	if( messageResult.result != SCE_COMMON_DIALOG_RESULT_OK )
		return Dialog::ID_Cancel;

	//This outer set of ifs is necessary because some of the button id enums share values.
	if( buttonSet == Dialog::BUTTONSET_CancelTryAgainOrContinue )
	{
		switch( messageResult.buttonId )
		{
		case SCE_MSG_DIALOG_BUTTON_ID_BUTTON1:
			return Dialog::ID_Continue;
		case SCE_MSG_DIALOG_BUTTON_ID_BUTTON2:
			return Dialog::ID_TryAgain;
		case SCE_MSG_DIALOG_BUTTON_ID_BUTTON3:
		case SCE_MSG_DIALOG_BUTTON_ID_INVALID:
		default:
			return Dialog::ID_Cancel;
		}
	}
	else if( buttonSet == Dialog::BUTTONSET_YesOrNo )
	{
		switch( messageResult.buttonId )
		{
		case SCE_MSG_DIALOG_BUTTON_ID_YES:
			return Dialog::ID_Yes;
		case SCE_MSG_DIALOG_BUTTON_ID_NO:
			return Dialog::ID_No;
		case SCE_MSG_DIALOG_BUTTON_ID_INVALID:
		default:
			return Dialog::ID_Cancel;
		}
	}
	else
	{
		switch( messageResult.buttonId )
		{
		case SCE_MSG_DIALOG_BUTTON_ID_OK:
			return Dialog::ID_OK;
		case SCE_MSG_DIALOG_BUTTON_ID_RETRY:
			return Dialog::ID_Retry;
		case SCE_MSG_DIALOG_BUTTON_ID_NO:
			return Dialog::ID_No;
		case SCE_MSG_DIALOG_BUTTON_ID_INVALID:
		default:
			return Dialog::ID_Cancel;
		}
	}

#else
	return Dialog::ID_Cancel;
#endif
}

//-----------------------------------------------------------------------------------------------
Dialog::ButtonID PopUpSystemDialog( const std::wstring& titleText, const std::wstring& messageText,
							Dialog::ButtonSet buttonSet, Dialog::Icon icon )
{
#if defined( PLATFORM_WINDOWS )
	return MessageBoxW( NULL, messageText.c_str(), titleText.c_str(), buttonSet | icon );
#else
	return Dialog::ID_Cancel;
#endif
}
