#pragma once

#include "ofMain.h"

class ofxHomography {
public:
	
	/*
	 * Homography Functions adapted from:
	 * http://www.openframeworks.cc/forum/viewtopic.php?p=22611
	 * Author: arturo castro
	 */
	static void gaussian_elimination(float *input, int n){
		// ported to c from pseudocode in
		// http://en.wikipedia.org/wiki/Gaussian_elimination
		
		float * A = input;
		int i = 0;
		int j = 0;
		int m = n-1;
		while (i < m && j < n){
			// Find pivot in column j, starting in row i:
			int maxi = i;
			for(int k = i+1; k<m; k++){
				if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
					maxi = k;
				}
			}
			if (A[maxi*n+j] != 0){
				//swap rows i and maxi, but do not change the value of i
				if(i!=maxi)
					for(int k=0;k<n;k++){
						float aux = A[i*n+k];
						A[i*n+k]=A[maxi*n+k];
						A[maxi*n+k]=aux;
					}
				//Now A[i,j] will contain the old value of A[maxi,j].
				//divide each entry in row i by A[i,j]
				float A_ij=A[i*n+j];
				for(int k=0;k<n;k++){
					A[i*n+k]/=A_ij;
				}
				//Now A[i,j] will have the value 1.
				for(int u = i+1; u< m; u++){
					//subtract A[u,j] * row i from row u
					float A_uj = A[u*n+j];
					for(int k=0;k<n;k++){
						A[u*n+k]-=A_uj*A[i*n+k];
					}
					//Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
				}
				
				i++;
			}
			j++;
		}
		
		//back substitution
		for(int i=m-2;i>=0;i--){
			for(int j=i+1;j<n-1;j++){
				A[i*n+m]-=A[i*n+j]*A[j*n+m];
				//A[i*n+j]=0;
			}
		}
	}
	
	
	template <class T>
	static void findHomography(T src, T dst, float homography[16]){
		
		// create the equation system to be solved
		// src and dst must implement [] operator for point access
		//
		// from: Multiple View Geometry in Computer Vision 2ed
		//       Hartley R. and Zisserman A.
		//
		// x' = xH
		// where H is the homography: a 3 by 3 matrix
		// that transformed to inhomogeneous coordinates for each point
		// gives the following equations for each point:
		//
		// x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
		// y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
		//
		// as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
		// so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
		// after ordering the terms it gives the following matrix
		// that can be solved with gaussian elimination:
		
		float P[8][9]={
			{-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
			{  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
			
			{-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
			{  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
			
			{-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
			{  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
			
			{-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
			{  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
		};
		
		gaussian_elimination(&P[0][0],9);
		
		// gaussian elimination gives the results of the equation system
		// in the last column of the original matrix.
		// opengl needs the transposed 4x4 matrix:
		float aux_H[]={ P[0][8],P[3][8],0,P[6][8],	// h11  h21 0 h31
                                P[1][8],P[4][8],0,P[7][8],	// h12  h22 0 h32
                                0      ,      0,1,0,		// 0    0   0 0
                                P[2][8],P[5][8],0,1};		// h13  h23 0 h33
		
		for(int i=0;i<16;i++) homography[i] = aux_H[i];
	}
	
	template <class T>
	static ofMatrix4x4 findHomography(T src, T dst){
		float homography[16];
		findHomography(src, dst, homography);
		return ofMatrix4x4(homography);
	}
	
	static ofVec2f toScreenCoordinates(ofVec2f point, ofMatrix4x4 homography)
	{
		ofVec3f point3(point);

		auto result = toScreenCoordinates(point3, homography);

		return ofVec2f(result.x, result.y);
	}
	static ofPoint toScreenCoordinates(ofPoint point, ofMatrix4x4 homography){
		ofVec4f original;
		ofVec4f screen;
		
		original.x = point.x;
		original.y = point.y;
		original.z = point.z;
		original.w = 1.0;
		
		ofMatrix4x4 transposed = ofMatrix4x4::getTransposedOf(homography);
		
		screen = transposed * original;
		
		screen.x = screen.x / screen.w;
		screen.y = screen.y / screen.w;
		screen.z = screen.z / screen.w;
		
		return ofPoint(screen.x,screen.y, screen.z);
	}

};

class HomoTrans
{
public:
	ofPoint originalCorners[4];
	ofPoint distortedCorners[4];
	ofMatrix4x4 homography;

	HomoTrans()
	{
		originalCorners[0].set(0, 0);
		originalCorners[1].set(1, 0);
		originalCorners[2].set(1, 1);
		originalCorners[3].set(0, 1);
	}

	void update()
	{
		homography = ofxHomography::findHomography(distortedCorners, originalCorners);
	}
};
