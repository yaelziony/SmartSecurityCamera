#include "Camera.h"

int main() {
	Camera& camera = Camera::getInstance();
	camera.runCamera();
	return 0;
}