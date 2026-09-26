# OTA_Bootloader_STM32F446RE
I started with a simple bootloader which validate the application using a magic number,application size and crc.all of those parameters are stored in a specific region of the flash memory"App_header",this region is accessed by both the application and the bootloader, the application write the parameters and the bootloader read it,if it is correct the bootloader jump directly to the application. then I started the Ota implementation by adding another parameter to the app header which is "Ota_flag", this Ota flag is set by pressing the user button of my board for simulate that an application update is requesting, when the application find the Ota flag is set ,it calls the Ota process that represented by these steps:
●Ota flag is set
●soft reset for returning to the bootloader 
●the bootloader clears the Ota flag
●the bootloader copy the new application to the flash memory from an hex file that contains the new application, its size,its CRC.
●the bootloader validate the new application but this time it is not read the bootloader parameters from the "app_header"but directly from the application image ,if the parameters are valid the bootloader store it in the "app_header",then it jumps to the new application.

<img width="900" height="1600" alt="image" src="https://github.com/user-attachments/assets/818cb62c-5c14-4b9a-8e03-830d5e563144" />


<img width="1600" height="167" alt="image" src="https://github.com/user-attachments/assets/36f7ea8f-a5c5-4f74-8695-8e6aa8235ff8" />


