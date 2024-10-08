#pragma once

struct vec_float4
{
	float body[4];
};

struct COORD2
{
	float x;
	float y;
};

struct COORD3
{
	float x;
	float y;
	float z;
};

struct MATRIX4
{
	union
	{
		struct
		{
			float m00;
			float m01;
			float m02;
			float m03;
			float m10;
			float m11;
			float m12;
			float m13;
			float m20;
			float m21;
			float m22;
			float m23;
			float m30;
			float m31;
			float m32;
			float m33;
		};
		vec_float4 r[4];
		float mData[4][4];
	};
};

struct tColor
{
	float mR;
	float mG;
	float mB;
	float mA;
};

struct tDebugText
{
	COORD2 mPos;
	tColor mColor;
	char mText[81];
	bool mRenderAlways;
	float mPersistTime;
	float mScale;
};

struct tLine
{
	COORD3 mFrom;
	COORD3 mTo;
	tColor mColorFrom;
	tColor mColorTo;
	bool mRenderAlways;
};

struct tDebugSphere
{
	COORD3 mPos;
	float mRadius;
	tColor mColor;
	float mPersistTime;
	bool mRenderAlways;
};

struct tDebugBox
{
	MATRIX4 mBox;
	tColor mColor;
	float mPersistTime;
	bool mRenderAlways;
};

struct tDebugCylinder
{
	MATRIX4 mCylinder;
	tColor mColor;
	float mPersistTime;
	bool mRenderAlways;
};

struct lalala
{
	int mNumDebugTextLines;
	tDebugText* mDebugText;
	int mNumDebugLines;
	tLine* mDebugLines;
	float* mLinePersistTime;
	int mNumDebugSpheres;
	tDebugSphere* mDebugSpheres;
	int mNumDebugBoxes;
	tDebugBox* mDebugBoxes;
	int mNumDebugCylinders;
	tDebugCylinder* mDebugCylinders;
};

struct cDebugRender
{
	float mLastElapsedTime;
	lalala mData[2];
	int mLineBufferHandles[3];
	int mCurrentLineBuffer;
};

void RenderText(float x, float y, tColor color, char* text, float persist_time, bool render_always, bool use_render_index, float scale);