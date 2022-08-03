//209409218 Dor Huri
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;

/*used macro instead of direct calculation*/
/* Compute min and max of two integers, respectively */
#define min(a, b)  (a < b ? a : b)
#define max(a, b)  (a > b ? a : b)
//for the row and col of image matrix
#define calcIndex(i,j,n) ((i)*(n)+(j))
//condition checking if num is (big or equal) or small
#define check(a,b) (a >= b ? 1 : 0)
//the complete calculation of blur rgb value
#define sumBlur(a,b,c,d,e,f,i,j,k) ((a+b+c+d+e+f+i+j+k)/9)
//the complete calculation of sharp rgb value
#define sumSharp(a,b,c,d,e,f,i,j,k) (-1*(a+b+c+d+f+i+j+k)+((e<<3)+e))
//the complete calculation of filter rgb value
#define sumFilter(a,b,c,d,e,f,i,j,k,l,m,n) (((a+b+c+d+e+f+i+j+k)-(l+m))/n)
//calculate num+1
#define pixOne(a) (a+1)
//calculate num+2
#define pixTwo(a) (a+2)
//calculate num+n
#define pixN(a) (a+n)
//calculate the row of the square
#define row(a,b) (a*b)
//calculate the rgb value of pixel
#define sumRgb(a,b,c) (a+b+c)
/*************************************************************************************
 * all the function of the original file is all in one function
 * doConvolution which contains all function in different implementation
 * that mostly remove unnecessary computation and uses macro and the register
 * keyword for saving access to the memory stack this function of curse compute for
 * both cases filter and unfiltered and preformed blur or sharp depended on input
 * @param image the original image.
 * @param kernelSize the size of the kernel matrix.
 * @param kernel the kernel matrix.
 * @param kernelScale the scale we dived with.
 * @param filter indicating using filter or not.
 ************************************************************************************/
void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {
    //creating variable
    register int i,j,colSquare;
    //allocating space for image in pixel
    register pixel* imageCalc = malloc(m*n*sizeof(pixel));
    //saving image
    register pixel* imagePixel = (pixel*)image->data;
    //imageCalc get the image inputme
    memcpy(imageCalc,imagePixel,(m*n*sizeof(pixel)));
    //for assigning each pixel in the loop
    register pixel pixel1;
    //case of unfiltered image
    if(!filter) {
        //if kernel consist 1 in cell[1,1] we are using blur
        if (kernel[1][1] == 1) {
            for (i = 1; i < n - 1; i = pixOne(i)) {
                colSquare=0;
                //the row of the selected square
                register int rowSquare = row((i-1),n);
                for (j = 1; j < n - 2; j = pixOne(j)) {
                    //the matrix we duplicate with the kernel assigning each pixel
                    register pixel *pix0_0 = &imageCalc[rowSquare + colSquare];
                    register pixel* pix0_1 = pixOne(pix0_0);
                    register pixel* pix0_2 = pixTwo(pix0_0);
                    register pixel* pix1_0 = pixN(pix0_0);
                    register pixel* pix1_1 = pixOne(pix1_0);
                    register pixel* pix1_2 = pixTwo(pix1_0);
                    register pixel* pix2_0 = pixN(pix1_0);
                    register pixel* pix2_1 = pixOne(pix2_0);
                    register pixel* pix2_2 = pixTwo(pix2_0);
                    //summing all the rgb value combine and dividing by kernel scale(which is 9)
                    register int redSum = sumBlur((int) pix0_0->red,(int) pix0_1->red,(int) pix0_2->red,
                                                  (int) pix1_0->red,(int) pix1_1->red,(int) pix1_2->red,
                                                  (int) pix2_0->red,(int) pix2_1->red,(int) pix2_2->red);
                    register int greenSum = sumBlur((int) pix0_0->green,(int) pix0_1->green,(int) pix0_2->green,
                                                    (int) pix1_0->green,(int) pix1_1->green,(int) pix1_2->green,
                                                    (int) pix2_0->green,(int) pix2_1->green,(int) pix2_2->green);
                    register int blueSum =sumBlur((int) pix0_0->blue,(int) pix0_1->blue,(int) pix0_2->blue,
                                                  (int) pix1_0->blue,(int) pix1_1->blue,(int) pix1_2->blue,
                                                  (int) pix2_0->blue,(int) pix2_1->blue,(int) pix2_2->blue);
                    //taking min for rgb value between 255 to sum rgb(only min because sum rgb positive)
                    pixel1.green = (unsigned char) min(greenSum, 255);
                    pixel1.red = (unsigned char) min(redSum, 255);
                    pixel1.blue = (unsigned char) min(blueSum, 255);
                    //assigning calculate pixel in image
                    imagePixel[calcIndex(i, j, n)] = pixel1;
                    colSquare = pixOne(colSquare);
                }
            }
        }
        //if kernel consist 9 in cell[1,1] we are using sharp
        else if(kernel[1][1] == 9) {
            for(i=1;i<n-1;i=pixOne(i)) {
                colSquare=0;
                //the row of the selected square
                register int rowSquare = row((i-1),n);
                for(j=1;j<n-2;j= pixOne(j)) {
                    //the matrix we duplicate with the kernel assigning each pixel
                    register pixel *pix0_0 = &imageCalc[rowSquare + colSquare];
                    register pixel* pix0_1 = pixOne(pix0_0);
                    register pixel* pix0_2 = pixTwo(pix0_0);
                    register pixel* pix1_0 = pixN(pix0_0);
                    register pixel* pix1_1 = pixOne(pix1_0);
                    register pixel* pix1_2 = pixTwo(pix1_0);
                    register pixel* pix2_0 = pixN(pix1_0);
                    register pixel* pix2_1 = pixOne(pix2_0);
                    register pixel* pix2_2 = pixTwo(pix2_0);
                    //summing all the rgb value combine
                    register int redSum = sumSharp((int) pix0_0->red,(int) pix0_1->red,(int) pix0_2->red,
                                                   (int) pix1_0->red,(int) pix1_1->red,(int) pix1_2->red,
                                                   (int) pix2_0->red,(int) pix2_1->red,(int) pix2_2->red);
                    register int greenSum = sumSharp((int) pix0_0->green,(int) pix0_1->green,(int) pix0_2->green,
                                                     (int) pix1_0->green,(int) pix1_1->green,(int) pix1_2->green,
                                                     (int) pix2_0->green,(int) pix2_1->green,(int) pix2_2->green);
                    register int blueSum =sumSharp((int) pix0_0->blue,(int) pix0_1->blue,(int) pix0_2->blue,
                                                   (int) pix1_0->blue,(int) pix1_1->blue,(int) pix1_2->blue,
                                                   (int) pix2_0->blue,(int) pix2_1->blue,(int) pix2_2->blue);
                    //taking min for rgb value between 255 and max of 0 and rgb sum
                    pixel1.green = (unsigned char) min(max(0,greenSum),255);
                    pixel1.red = (unsigned char) min(max(0,redSum),255);
                    pixel1.blue = (unsigned char) min(max(0,blueSum),255);
                    //assigning calculate pixel in image
                    imagePixel[calcIndex(i,j,n)] = pixel1;
                    colSquare = pixOne(colSquare);
                }
            }
        }
    }
    //in case we ar using filter
    else if(filter) {
        if(kernel[0][0] == 1) {
            //for summing each pixel rgb value
            register int sum;
            for (i = 1; i < n - 1; i = pixOne(i)) {
                colSquare=0;
                //the row of the selected square
                register int rowSquare = row((i-1),n);
                for (j = 1; j < n - 2; j = pixOne(j)) {
                    //the matrix we duplicate with the kernel assigning each pixel
                    register pixel *pix0_0 = &imageCalc[rowSquare + colSquare];
                    register pixel* pix0_1 = pixOne(pix0_0);
                    register pixel* pix0_2 = pixTwo(pix0_0);
                    register pixel* pix1_0 = pixN(pix0_0);
                    register pixel* pix1_1 = pixOne(pix1_0);
                    register pixel* pix1_2 = pixTwo(pix1_0);
                    register pixel* pix2_0 = pixN(pix1_0);
                    register pixel* pix2_1 = pixOne(pix2_0);
                    register pixel* pix2_2 = pixTwo(pix2_0);
                    //first pixel rgb value in matrix
                    register int max = sumRgb((int)pix0_0->red , (int)pix0_0->green , (int)pix0_0->blue), min = max;
                    register pixel* pixMin = pix0_0;
                    register pixel* pixMax = pix0_0;
                    //8 if else condition, so we get the pixel with max and min value in matrix
                    sum = sumRgb((int)pix0_1->red ,(int)pix0_1->green , (int)pix0_1->blue);
                    if(check(min,sum)) {
                        min = sum;
                        pixMin = pix0_1;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix0_1;
                    }
                    sum = sumRgb((int)pix0_2->red,(int)pix0_2->green ,(int)pix0_2->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix0_2;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix0_2;
                    }
                    sum = sumRgb((int)pix1_0->red,(int)pix1_0->green,(int)pix1_0->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix1_0;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix1_0;
                    }
                    sum = sumRgb((int)pix1_1->red , (int)pix1_1->green , (int)pix1_1->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix1_1;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix1_1;
                    }
                    sum = sumRgb((int)pix1_2->red , (int)pix1_2->green , (int)pix1_2->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix1_2;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix1_2;
                    }
                    sum = sumRgb((int)pix2_0->red ,(int)pix2_0->green ,(int)pix2_0->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix2_0;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix2_0;
                    }
                    sum = sumRgb((int)pix2_1->red , (int)pix2_1->green , (int)pix2_1->blue);
                    if(check(min,sum)){
                        min = sum;
                        pixMin = pix2_1;
                    }
                    else if(!check(max,sum)){
                        max = sum;
                        pixMax = pix2_1;
                    }
                    sum = sumRgb((int)pix2_2->red ,(int)pix2_2->green , (int)pix2_2->blue);
                    if(check(min,sum)){
                        pixMin = pix2_2;
                    }
                    else if(!check(max,sum)){
                        pixMax = pix2_2;
                    }
                    //summing all the rgb value combine minus the min and max pixel rgb value
                    // and dividing by kernel scale(which is the filter)
                    register int redSum = sumFilter((int) pix0_0->red,(int) pix0_1->red,(int) pix0_2->red,
                                                  (int) pix1_0->red,(int) pix1_1->red,(int) pix1_2->red,
                                                  (int) pix2_0->red,(int) pix2_1->red,(int) pix2_2->red,(int)pixMax->red,
                                                  (int)pixMin->red,kernelScale);
                    register int greenSum = sumFilter((int) pix0_0->green,(int) pix0_1->green,(int) pix0_2->green,
                                                    (int) pix1_0->green,(int) pix1_1->green,(int) pix1_2->green,
                                                    (int) pix2_0->green,(int) pix2_1->green,(int) pix2_2->green,(int)pixMax->green,
                                                      (int)pixMin->green,kernelScale);
                    register int blueSum =sumFilter((int) pix0_0->blue,(int) pix0_1->blue,(int) pix0_2->blue,
                                                  (int) pix1_0->blue,(int) pix1_1->blue,(int) pix1_2->blue,
                                                  (int) pix2_0->blue,(int) pix2_1->blue,(int) pix2_2->blue,(int)pixMax->blue,
                                                    (int)pixMin->blue,kernelScale);
                    //taking min for rgb value between 255 to sum rgb(only min because sum rgb positive)
                    pixel1.green = (unsigned char) min(greenSum,255);
                    pixel1.red = (unsigned char) min(redSum,255);
                    pixel1.blue = (unsigned char) min(blueSum,255);
                    //assigning calculate pixel in image
                    imagePixel[calcIndex(i,j,n)] = pixel1;
                    colSquare = pixOne(colSquare);
                }
            }
        }
        //if kernel consist 9 in cell[1,1] we are using sharp
        else if(kernel[1][1] == 9) {
        for(i=1;i<n-1;i= pixOne(i)) {
            colSquare=0;
            //the row of the selected square
            register int rowSquare = row((i-1),n);
            for(j=1;j<n-2;j=pixOne(j)) {
                //the matrix we duplicate with the kernel assigning each pixel
                register pixel *pix0_0 = &imageCalc[rowSquare + colSquare];
                register pixel* pix0_1 = pixOne(pix0_0);
                register pixel* pix0_2 = pixTwo(pix0_0);
                register pixel* pix1_0 = pixN(pix0_0);
                register pixel* pix1_1 = pixOne(pix1_0);
                register pixel* pix1_2 = pixTwo(pix1_0);
                register pixel* pix2_0 = pixN(pix1_0);
                register pixel* pix2_1 = pixOne(pix2_0);
                register pixel* pix2_2 = pixTwo(pix2_0);
                //summing all the rgb value combine
                register int redSum = sumSharp((int) pix0_0->red,(int) pix0_1->red,(int) pix0_2->red,
                                              (int) pix1_0->red,(int) pix1_1->red,(int) pix1_2->red,
                                              (int) pix2_0->red,(int) pix2_1->red,(int) pix2_2->red);
                register int greenSum = sumSharp((int) pix0_0->green,(int) pix0_1->green,(int) pix0_2->green,
                                                (int) pix1_0->green,(int) pix1_1->green,(int) pix1_2->green,
                                                (int) pix2_0->green,(int) pix2_1->green,(int) pix2_2->green);
                register int blueSum =sumSharp((int) pix0_0->blue,(int) pix0_1->blue,(int) pix0_2->blue,
                                              (int) pix1_0->blue,(int) pix1_1->blue,(int) pix1_2->blue,
                                              (int) pix2_0->blue,(int) pix2_1->blue,(int) pix2_2->blue);
                //taking min for rgb value between 255 and max of 0 and rgb sum
                pixel1.green = (unsigned char) min(max(0,greenSum),255);
                pixel1.red = (unsigned char) min(max(0,redSum),255);
                pixel1.blue = (unsigned char) min(max(0,blueSum),255);
                //assigning calculate pixel in image
                imagePixel[calcIndex(i,j,n)] = pixel1;
                colSquare = pixOne(colSquare);
                }
            }
        }
    }
}
void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	if (flag == '1') {	
		// blur image
		doConvolution(image, 3, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution(image, 3, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution(image, 3, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

