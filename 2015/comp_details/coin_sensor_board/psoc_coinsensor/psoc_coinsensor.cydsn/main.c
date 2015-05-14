/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main()
{
    uint16_t rawReading;
    CyGlobalIntEnable;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */    
    CapSense_1_Start();
    CapSense_1_InitializeAllBaselines();
    CapSense_1_EnableWidget(0);
    
    for(;;)
    {
        CapSense_1_UpdateEnabledBaselines();	
        CapSense_1_ScanEnabledWidgets();
        LED1_Write(1);
        CyDelay(100);
        CapSense_1_UpdateEnabledBaselines();	
        CapSense_1_ScanEnabledWidgets();
        LED1_Write(0);
        LED0_Write(1);
        CyDelay(100);
        CapSense_1_UpdateEnabledBaselines();	
        CapSense_1_ScanEnabledWidgets();
        LED0_Write(0);

        while(CapSense_1_IsBusy());
        
        rawReading = CapSense_1_GetDiffCountData(0);
        
        if (rawReading > 0x200){
            LED1_Write(0);
            while(1);
        }
        
    }
}

/* [] END OF FILE */
