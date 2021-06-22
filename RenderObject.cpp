#include "RenderObject.h"


#include <GLUT/glut.h>

RenderObject::RenderObject(const char *filename)
{
	m = new Molecule(filename);
	
	setRotationCount(300, 5);
	
	qStart = randomUnitQuaternion();
	resetQuaternions(qStart);
}

RenderObject::~RenderObject()
{
	delete m;
}

void RenderObject::setRotationCount(int rc, int rw)
{
	rotationCount = rc;
	rotationWidth = rw;
}

void RenderObject::initializeGL()
{
	GLfloat fogColor[4] = {0.0, 0.0, 0.0, 0.0}; 
	glEnable(GL_FOG);
	
	glFogfv(GL_FOG_COLOR, fogColor);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	
	glEnable (GL_DEPTH_TEST);
	
	glEnable(GL_LINE_SMOOTH);
	
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  
}

void RenderObject::setProjection(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
    glViewport(0, 0, w, h);

	gluPerspective(45,1.0*w/h,1e-3,1e3);
}

void RenderObject::resetQuaternions(const Quaternion<float>& qRot)
{
	qEnd = 1*qRot + 1*randomUnitQuaternion();
	qEnd = qEnd / qEnd.abs();

	frameCount = 0;
	maxRotationFrames = rotationCount * (qEnd - qStart).abs();
}

void RenderObject::render(float percentDone, int mode)
{	
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	gluLookAt(0.0,0.0,0.8, 0.0,0.0,0.0, 0.0f,1.0f,0.0f);
	
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 1.2);
	
	// do rotation
	/*Slerp<Quaternion<float>, float> slerp(qStart, qEnd);
	Quaternion<float> qRot = slerp.value(1.0 * frameCount / maxRotationFrames);
	float rotation[16];
	convertQuaternionToRotation(rotation, qRot);
	glMultMatrixf(rotation);*/
	
	//glRotatef(360.0 * frameCount / (rotationWidth*maxRotationFrames), 0, 1, 0);
	glRotatef(frameCount, 0,1,0);

	// do zoom
	/*float sinfactor = sin(1.0 * zoomCount / 100 );
	float sizeScale = 1.0 + 0.5 * sinfactor * sinfactor;
	glScalef(sizeScale, sizeScale, sizeScale);*/
	
	/*float blendWindow = 0.02;
	if (percentDone < 0.5 - blendWindow)
	{
		m->render(false);
	}
	else if (percentDone >= (0.5 - blendWindow) && percentDone < 0.5)
	{
		m->render(false);
		RenderObject::fade(1.0 - (0.5 - percentDone)/blendWindow);
	}
	else if (percentDone >= (0.5) && percentDone < (0.5 + blendWindow))
	{
		m->render(true);
		RenderObject::fade(1.0 - (percentDone - 0.5)/blendWindow);
	}
	else
	{
		m->render(true);
	}*/
	
	
	m->render(mode);
	
	// increment the frame count
	frameCount++;
	zoomCount++;
	
	// choose a new direction
	/*if (frameCount == (rotationWidth*maxRotationFrames))
	{
		resetQuaternions(qStart = qRot);
	}*/
}


Quaternion<float> randomUnitQuaternion()
{
	Quaternion<float> q = Quaternion<float>::random_imag();
	return q / q.abs();
}

void convertQuaternionToRotation(float *result, const Quaternion<float>& q)
{
	float x = q.imag_i();
	float y = q.imag_j();
	float z = q.imag_k();
	float w = q.real();

	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;
 
	result[0] = 1.0f - 2.0f * (y2 + z2);
	result[1] = 2.0f * (xy - wz);
	result[2] = 2.0f * (xz + wy);
	result[3] = 0.0f;
	result[4] = 2.0f * (xy + wz);
	result[5] = 1.0f - 2.0f * (x2 + z2);
	result[6] = 2.0f * (yz - wx);
	result[7] = 0.0f;
	result[8] = 2.0f * (xz - wy);
	result[9] = 2.0f * (yz + wx);
	result[10] = 1.0f - 2.0f * (x2 + y2);
	result[11] = 0.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
}

void RenderObject::fade(float val)
{
	glColor4f(0,0,0,val);
	glMatrixMode (GL_MODELVIEW); 
	glPushMatrix (); 
	glLoadIdentity (); 
	glMatrixMode (GL_PROJECTION); 
	glPushMatrix (); 
	glLoadIdentity ();
	glBegin (GL_QUADS); 
	glVertex3i (-1, -1, -1); 
	glVertex3i (1, -1, -1); 
	glVertex3i (1, 1, -1); 
	glVertex3i (-1, 1, -1); 
	glEnd ();
	glPopMatrix (); 
	glMatrixMode (GL_MODELVIEW); 
	glPopMatrix ();
}