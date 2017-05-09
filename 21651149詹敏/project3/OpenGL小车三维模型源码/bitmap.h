#include <stdlib.h> 
#include <windows.h> 
#include <stdio.h> 
 
#define BITMAP_ID   0x4D42    // the universal bitmap ID 
/***************************************************************************** 
 LoadBitmapFile() 
 
 Returns a pointer to the bitmap image of the bitmap specified by filename. 
 Also returns the bitmap header information. No support for 8-bit bitmaps. 
*****************************************************************************/ 
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) 
{ 
  FILE *filePtr;                        // the file pointer 
  BITMAPFILEHEADER  bitmapFileHeader;   // bitmap file header 
  unsigned char    *bitmapImage;        // bitmap image data 
  unsigned int      imageIdx = 0;       // image index counter 
  unsigned char     tempRGB;            // swap variable 
 
  // open filename in "read binary" mode 
  filePtr = fopen(filename, "rb"); 
  if (filePtr == NULL) 
    return NULL; 
 
  // read the bitmap file header 
  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr); 
 
  // verify that this is a bitmap by checking for the universal bitmap id 
  if (bitmapFileHeader.bfType != BITMAP_ID) 
  { 
    fclose(filePtr); 
    return NULL; 
  } 
 
  // read the bitmap information header 
  fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); 
 
  // move file pointer to beginning of bitmap data 
  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET); 
 
  // allocate enough memory for the bitmap image data 
  bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage); 
 
  // verify memory allocation 
  if (!bitmapImage) 
  { 
    free(bitmapImage); 
    fclose(filePtr); 
    return NULL; 
  } 
 
  // read in the bitmap image data 
  fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr); 
 
  // make sure bitmap image data was read 
  if (bitmapImage == NULL) 
  { 
    fclose(filePtr); 
    return NULL; 
  } 
 
  // swap the R and B values to get RGB since the bitmap color format is in BGR 
  for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3) 
  { 
    tempRGB = bitmapImage[imageIdx]; 
    bitmapImage[imageIdx] = bitmapImage[imageIdx + 2]; 
    bitmapImage[imageIdx + 2] = tempRGB; 
  } 
 
  // close the file and return the bitmap image data 
  fclose(filePtr); 
  return bitmapImage; 
} // end LoadBitmapFile() 
 
 
/***************************************************************************** 
 LoadBitmapFileWithAlpha 
 
 Loads a bitmap file normally, and then adds an alpha component to use for 
 blending 
*****************************************************************************/ 
unsigned char *LoadBitmapFileWithAlpha(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) 
{ 
  unsigned char *bitmapImage = LoadBitmapFile(filename, bitmapInfoHeader); 
  unsigned char *bitmapWithAlpha = (unsigned char *)malloc(bitmapInfoHeader->biSizeImage * 4 / 3); 
 
  if (bitmapImage == NULL || bitmapWithAlpha == NULL) 
    return NULL; 
 
  // loop through the bitmap data 
  for (unsigned int src = 0, dst = 0; src < bitmapInfoHeader->biSizeImage; src +=3, dst +=4) 
  { 
    // if the pixel is black, set the alpha to 0. Otherwise, set it to 255. 
    if (bitmapImage[src] == 0 && bitmapImage[src+1] == 0 && bitmapImage[src+2] == 0) 
      bitmapWithAlpha[dst+3] = 0; 
    else 
      bitmapWithAlpha[dst+3] = 0xFF; 
 
    // copy pixel data over 
    bitmapWithAlpha[dst] = bitmapImage[src]; 
    bitmapWithAlpha[dst+1] = bitmapImage[src+1]; 
    bitmapWithAlpha[dst+2] = bitmapImage[src+2]; 
  } 
 
  free(bitmapImage); 
 
  return bitmapWithAlpha; 
} // end LoadBitmapFileWithAlpha() 

