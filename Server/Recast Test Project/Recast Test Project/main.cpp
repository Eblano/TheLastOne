#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Recast.h"
#include "MeshLoaderObj.h"
#include "InputGeom.h"

#ifdef WIN32
#	define snprintf _snprintf
#	define putenv _putenv
#endif

int main() {
	// �Ž� ������ üũ
	rcMeshLoaderObj mesh;
	InputGeom* geom = 0;
	rcContext ctx;

	//------------------------
	// ����Ƽ ���� �÷��̾� ��ġ x, z
	int mx = 1009;
	int my = 1150;
	//------------------------
	// ���� ��ǥ�� �ϴ� ��ġ x, z
	int tx = 328;
	int ty = 801;
	//------------------------

	std::cout << mesh.load("./navmesh/InGame.obj") << std::endl;

	geom = new InputGeom;
	geom->loadMesh("./navmesh/InGame.obj");


}
