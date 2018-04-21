#include <ansi_c.h>
#include "color_picker.h"
#include <utility.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include "ArduinoTest1.h"

static int panelHandle;
static int panelHandleHelp;
static int OpenedComPort;
static int OpenComPortStatus=0; 
#define MatrixHeight 7
#define MatrixWeight 7 

/// prototypes//////

static void				ConfigureColorSelector (void);
static int CVICALLBACK	SelectorCB (int panelHandle, int controlID, int event, void *callbackData,
							int eventData1, int eventData2, int eventData );

void GetMatrixData(char *MartixDATA) ;
int ColorToLetter(void);
void tohex(unsigned char * in, size_t insz, char * out, size_t outsz) ;

///////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ArduinoTest1.uir", PANEL)) < 0)
		return -1;
	if ((panelHandleHelp = LoadPanel(0, "ArduinoTest1.uir", PANEL_HELP)) < 0)
        {
        DiscardPanel (panelHandle);
        return -1;
        }
	
	ColorPicker_Convert (panelHandle, PANEL_CanvasColorPicker, 0, 0, 0); 
	ConfigureColorSelector();
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	DiscardPanel (panelHandleHelp);
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK OnOpenComPortPressed (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	int iSelectedComPort = 0 ;
	int iComPortOpenError = 0 ; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_iLBComPortList, &iSelectedComPort);
			DisableBreakOnLibraryErrors ();   
			iComPortOpenError = OpenComConfig (iSelectedComPort, "", 9600, 0, 8, 1, 512, 512);
			if(iComPortOpenError>=0)
			{
				//success
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "Com Port Open\r\n");
				OpenComPortStatus=1;
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_bSendLedColorAndPlace, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_bSENDMATRIX, ATTR_DIMMED, 0) ;
				SetCtrlAttribute (panelHandle, PANEL_bSENDTEXT, ATTR_DIMMED, 0) ;
				SetCtrlAttribute (panelHandle, PANEL_bClearAll, ATTR_DIMMED, 0) ;
				OpenedComPort = iSelectedComPort;
			} 
			else
			{
				//com port open failed
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "Com Port Fail Open\r\n");
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, GetRS232ErrorString (iComPortOpenError) );
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "\r\n");
			}
			

			break;
	}
	return 0;
}

int CVICALLBACK OnCloseComPortPressed (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	int iCloseComPortError = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if (OpenComPortStatus == 1 )
			{
				iCloseComPortError = CloseCom(OpenedComPort);
				if(iCloseComPortError>=0)
				{
					//success
					OpenComPortStatus=0;
					SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "Com Port Closed\r\n");
					SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON, ATTR_DIMMED, 0);
					SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 1);
					SetCtrlAttribute (panelHandle, PANEL_bSendLedColorAndPlace, ATTR_DIMMED, 1);
					SetCtrlAttribute (panelHandle, PANEL_bSENDMATRIX, ATTR_DIMMED, 1) ;
					SetCtrlAttribute (panelHandle, PANEL_bSENDTEXT, ATTR_DIMMED, 1) ;
					SetCtrlAttribute (panelHandle, PANEL_bClearAll, ATTR_DIMMED, 1) ; 
					
					
				}
				else
				{
				//com port close failed
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "Com Port Fail Close\r\n");
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, GetRS232ErrorString (iCloseComPortError) );
				SetCtrlVal (panelHandle, PANEL_sTBComPortStatus, "\r\n"); 
				}
			}
			break;
	}
	return 0;
}
///////////////// this is for testing communication with the arduino 
int CVICALLBACK OnLedPressed (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int iButtonState=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_iBLED, &iButtonState);
			if (OpenComPortStatus == 1)
			{
				if( iButtonState==1)
				{
					ComWrt (OpenedComPort, "H" , 1);
				}
				else
				{
					ComWrt (OpenedComPort, "L" , 1); 
				}
			}
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////
int CVICALLBACK bSendLedColorAndPlace (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{   
	char iLedNumber=0;
	int rLedColor=0;
	char Data[4];
	switch (event)
	{
		case EVENT_COMMIT:
			ComWrt (OpenedComPort, "S" , 1);  // Sets the mode LED control
			rLedColor = ColorToLetter();
			GetCtrlVal (panelHandle, PANEL_LEDNumber, &iLedNumber);
			
			// from int to char size for each color

			Data[0] = ((rLedColor>>16) & 0xFF) ; //red
			Data[1] = ((rLedColor>>8) & 0xFF) ; //green 
			Data[2] =  ((rLedColor)& 0xFF) ; 	//blue
			
			
			Data[3] =  iLedNumber;
			
			ComWrt (OpenedComPort, Data, 4);
			
			break;
	}
	return 0;
}

int ColorToLetter()
{
	int iLedColor=0;
	GetCtrlVal (panelHandle, PANEL_LedColorList, &iLedColor); 
		switch(iLedColor)
		{

			case 1: return VAL_RED;   //RED
			case 2 : return VAL_DK_BLUE;   //BLUE
			case 3 :return VAL_GREEN;  //GREEN

			default: break;

		}
	return ;
	
}

int CVICALLBACK bPaintRedPressed (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	Point activeCell; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetActiveTableCell (panelHandle, PANEL_TABLE, &activeCell); 
			SetTableCellAttribute (panelHandle, PANEL_TABLE, activeCell, ATTR_TEXT_BGCOLOR, VAL_RED);
			break;
	}
	return 0;
}


int CVICALLBACK bPaintBluePressed (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	Point activeCell;
	switch (event)
	{
		case EVENT_COMMIT:
			GetActiveTableCell (panelHandle, PANEL_TABLE, &activeCell);
			SetTableCellAttribute (panelHandle, PANEL_TABLE, activeCell, ATTR_TEXT_BGCOLOR, VAL_BLUE);
			break;
	}
	return 0;
}

int CVICALLBACK bPaintGreenPressed (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	Point activeCell; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetActiveTableCell (panelHandle, PANEL_TABLE, &activeCell);
			SetTableCellAttribute (panelHandle, PANEL_TABLE, activeCell, ATTR_TEXT_BGCOLOR, VAL_GREEN);
			break;
	}
	return 0;
}

int CVICALLBACK bPaintColorPicker (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	Rect activeCells;
	int color=0;
	int i,k;
	switch (event)
	{
		case EVENT_COMMIT:
			GetTableSelection (panelHandle, PANEL_TABLE, &activeCells );
			ColorPicker_GetColor (panelHandle, PANEL_CanvasColorPicker, &color);
			for(k=activeCells.top;k<activeCells.height+activeCells.top;k++)
			{
				for(i=activeCells.left;i<activeCells.width+activeCells.left;i++)
				{
					SetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint(i,k) , ATTR_TEXT_BGCOLOR, color);  	
				}
			}
			break;
	}
	return 0;
}


int CVICALLBACK bSENDMATRIX (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	char tableData[4];
	int CellColor=0;
	int i,k;
	char pos;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ComWrt (OpenedComPort, "M" , 1);  // Sets the mode LED control 
			pos=0;
			for( i=1;i<8;i++)
			{
				for(k=1 ;k<8;k++)
				{
					GetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint (k, i), ATTR_TEXT_BGCOLOR, &CellColor);
					tableData[0] = ((CellColor>>16) & 0xFF) ; //red
					tableData[1] = ((CellColor>>8) & 0xFF) ; //green 
					tableData[2] =  ((CellColor)& 0xFF) ; 	//blue
					if((i%2)==0)   ///////////////matrix is connected in ZIGZAG
					{
						tableData[3] =  (char)((7*i)-k);
					}
					if((i%2)!=0) 
					{
						tableData[3] =  pos; 
					}
					ComWrt (OpenedComPort, tableData, 4);
					pos++;
					Delay(0.1);
					
				}

			}
			break;
	}
	return 0;
}

int CVICALLBACK bSENDTEXT (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	char String[20];
	int length=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute (panel, PANEL_STRING, ATTR_STRING_TEXT_LENGTH, &length);
			if (length >20 )
			{		
				MessagePopup ("Overflow detected", "Overflow Detected! Maximum characters are 20"); 
			}
			else
			{
				ComWrt (OpenedComPort, "T" , 1);  // Sets the mode TEXT control
				GetCtrlVal (panelHandle, PANEL_STRING, String);
				ComWrt (OpenedComPort, String, 20); 
			}
				
	}
	return 0;
}

static void ConfigureColorSelector (void)
{
	// register an event handler for color picker events
	ColorPicker_InstallCallback (panelHandle, PANEL_CanvasColorPicker, SelectorCB, NULL);
	
	// hide the transparent item, since PlotIntensity does not allow for VAL_TRANSPARENT in the colormap
	ColorPicker_SetAttribute (panelHandle, PANEL_CanvasColorPicker, ATTR_PICKER_TRANSPARENT_ITEM_VISIBLE, 0);
	

	return;
}

static int CVICALLBACK SelectorCB (int panelHandle, int controlID, int event, void *callbackData,
	int eventData1, int eventData2, int eventData3)
{
	int	color = 0;

	switch (event)
	{
		case PICKER_EVENT_COLOR_CHANGE:
			// eventData3 holds the new color that the user picked
			color = eventData3;
			break;
		case PICKER_EVENT_COLOR_SELECT:
			if (eventData2 == PICKER_VAL_FIXED_COLOR)
				// get the color of the selected cell. eventData1 holds the index of the selected cell
				ColorPicker_GetFixedColor (panelHandle, controlID, eventData1, &color);
			else	// PICKER_VAL_CUSTOM_COLOR
				// get the color of the selected cell. eventData1 holds the index of the selected cell
				ColorPicker_GetCustomColor (panelHandle, controlID, eventData1, &color);
			break;
	}
	
	return 0;
}


int CVICALLBACK bTABLEPressedCell (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{  	
	Point activeCell;
	int color=0;
	switch (event)
	{
		case EVENT_ACTIVE_CELL_CHANGE:
			GetActiveTableCell (panelHandle, PANEL_TABLE, &activeCell);
			ColorPicker_GetColor (panelHandle, PANEL_CanvasColorPicker, &color);
			SetTableCellAttribute (panelHandle, PANEL_TABLE, activeCell, ATTR_TEXT_BGCOLOR, color);
			break;
	}
	return 0;
}

int CVICALLBACK bClearAllPressed (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ComWrt (OpenedComPort, "C" , 1);
			Delay(0.1);
			break;
	}
	return 0;
}

int CVICALLBACK bPaintRandomPressed (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	char i,k;
	int color;
	switch (event)
	{
		case EVENT_COMMIT:
			
			for( i=1;i<8;i++)
			{
				for(k=1 ;k<8;k++)
				{
					color = MakeColor (rand() / (double)RAND_MAX * 255,rand() / (double)RAND_MAX * 255, rand() / (double)RAND_MAX * 255);
					SetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint(i,k), ATTR_TEXT_BGCOLOR,color );
				}
			}
			break;;
	}
	return 0;
}

int CVICALLBACK bPaintHelpPressed (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				     //Display the panel and wait for any UI action 
				    InstallPopup (panelHandleHelp);
				    GetUserEvent (1, 0, 0);  
				    RemovePopup (panelHandleHelp);

			break;
	}
	return 0;
}


int CVICALLBACK bPaintClearMatrixPressed (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char i,k;
	int color;
	switch (event)
	{
		case EVENT_COMMIT:
			
			for( i=1;i<8;i++)
			{
				for(k=1 ;k<8;k++)
				{
					color = VAL_BLACK ;
					SetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint(i,k), ATTR_TEXT_BGCOLOR,color );
				}
			}
			break;;
	}
	return 0;
}

int CVICALLBACK bGetCodePressed (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int CellColor=0;
	int i,k;
	unsigned char ColorData[3];
	char str[6];
	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox (panelHandle, PANEL_sCODE, "");
			for( i=1;i<8;i++)
			{
				for(k=1 ;k<8;k++)
				{
					GetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint (k, i), ATTR_TEXT_BGCOLOR, &CellColor);

					ColorData[0] = ((CellColor>>16) & 0xFF) ; //red
					ColorData[1] = ((CellColor>>8) & 0xFF) ; //green 
					ColorData[2] =  ((CellColor)& 0xFF) ; 	//blue
					
					tohex(ColorData, 3, str, 6);
					
					SetCtrlVal (panelHandle, PANEL_sCODE, "0x"); 
					SetCtrlVal (panelHandle, PANEL_sCODE, str);
					
					if (i==7 && k==7) 
					{
						//last ColorData can add actions
					}
					else
						SetCtrlVal (panelHandle, PANEL_sCODE, " ,");
				}
				if (i==7 && k==7) 
				{
						//last ColorData can add actions
				}
				else
					SetCtrlVal (panelHandle, PANEL_sCODE, "\n");
				 
			}
			break;
	}
	return 0;
}


void tohex(unsigned char * in, size_t insz, char * out, size_t outsz)
{
    unsigned char * pin = in;
    const char * hex = "0123456789ABCDEF";
    char * pout = out;
    for(; pin < in+insz; pout +=2, pin++)
	{
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        if (pout + 2 - out > outsz)
		{
            break;   //make sure that the destination is big enough
        }
    }
    pout[-1] = 0;
}

int CVICALLBACK bCopyCodePressed (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
    int length;
    char *text;
 
	switch (event)
	{
		case EVENT_COMMIT:
						GetCtrlAttribute (panel, PANEL_sCODE, ATTR_STRING_TEXT_LENGTH, &length);
					    if ((text = (char *)malloc (length + 1)) != NULL)
					    {
					        GetCtrlVal (panel, PANEL_sCODE, text);  
					        ClipboardPutText (text);
					        free (text);            
					     }
			break;
	}
	return 0;
}
