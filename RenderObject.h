#ifndef _RENDER_OBJECT_H_
#define _RENDER_OBJECT_H_

#include "Molecule.h"
#include "Quaternion.h"
#include "Slerp.h"


class RenderObject
{
	public:
		static void initializeGL();
	
		RenderObject(const char *filename);
		~RenderObject();
		
		void setRotationCount(int rc, int rw);
		//void setDisplayPeriod(int fade, int total);
		
		void render(float percentDone, int mode);

		void setProjection(int w, int h);
		
		static void fade(float val);
		
		Molecule *m;
		
	private:
		void resetQuaternions(const Quaternion<float>& qRot);

		int frameCount, maxRotationFrames, rotationCount, rotationWidth;
		int zoomCount;
		Quaternion<float> qStart, qEnd;
		
};

void convertQuaternionToRotation(float *result, const Quaternion<float>& q);
Quaternion<float> randomUnitQuaternion();

#endif