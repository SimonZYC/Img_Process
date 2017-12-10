# Img_Process
There are small projects for a course named Digital Image Processing. We students were asked to finish several projects with MFC or Win32 and implement some tasks about simple image processing.

My work was implemented with Win32 API.

##Project 1
The task of Project 1 is to open and read a **bmp gray scale** image and then show it in a window. 

The most important is to know the structure of BMP image, especially **BITMAPFILEHEADER**, **BITMAPINFOHEADER** and so on. For detailed information, you can refer to the comments in the code.

For displaying the image, I use `Bitblt()` instead of `setPixel()`, for the latter is quite inefficient.



