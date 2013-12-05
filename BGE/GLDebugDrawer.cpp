#include "GLDebugDrawer.h"
#include "LineDrawer.h"
#include "Game.h"
#include "Utils.h"

using namespace BGE;

GLDebugDrawer::GLDebugDrawer(void)
{

}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//only need this method for basic functionality
	LineDrawer::Instance()->DrawLine(BtToGLVector(to),BtToGLVector(from),BtToGLVector(color));
}

void GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	Game::Instance()->PrintText(warningString);
}

void GLDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{}

void GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{}

void GLDebugDrawer::setDebugMode(int debugMode)
{}

 int GLDebugDrawer::getDebugMode()const{
	return 0;
}

GLDebugDrawer::~GLDebugDrawer(void)
{
}
