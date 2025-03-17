// Kinematics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define JGL2_IMPLEMENTATION
#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <JGL2/JGL.hpp>
#include <JGL2/Simulation3DView.hpp>

using namespace JGL2;
using namespace jm;

Simulation3DView<JR::PBRRenderer>* view = nullptr;

struct link {
	float angle = 0;
	vec3 l;
	int parent = -1;
	mat4 transform(const std::vector<link>& links) const {
		if (parent >= 0) {
			return links[parent].transform(links) * rotate(angle, vec3(0, 0, 1)) * translate(l);
		}
		else {
			return translate(l)*rotate(angle, vec3(0, 0, 1));

		}
	}
	vec3 getPosition(const std::vector<link>& links) const {
		return transform(links) * vec4(0, 0, 0, 1);
	}
};

std::vector<link> links;
int picked = -1;


void init() {
	links.clear();
	links.push_back({ 0,vec3(0,30,0),-1 });
	links.push_back({ 0,vec3(0,20,0),0 });
	links.push_back({ 0.2,vec3(0,30,0),1 });
	links.push_back({ -0.2,vec3(0,20,0),1 });
}
void frame(float dt) {
	links[0].angle += 0.03;
}


void render() {

	for (auto& l : links) {
		vec3 p0 = l.getPosition(links);
		JR::drawSphere(p0, 1,vec4(1,.5,0,1));
		if (l.parent >= 0) {
			vec3 pp = links[l.parent].getPosition(links);
			JR::drawCylinder(p0, pp,0.6,vec4(1,0,0,1));
		}
	}
	if( picked>=0 )
		JR::drawSphere( links[picked].getPosition(links), 1, vec4(0,1,0,1) );

	JR::drawQuad(jm::vec3(0), jm::vec3(0, 1, 0), jm::vec2(1000), jm::vec4(0, 0, .4, 1));
}

bool move3D( const vec3& p ) {
	return false;
}

bool drag3D( const vec3& delta ) {
	return false;
}

int main() {
	Window* win = new Window(800,600,"IK");
	win->alignment(align_t::ALL);
	view = new Simulation3DView<JR::PBRRenderer>(0,0,800,600,"View");
	view->initCB(init);
	view->move3DCB(move3D);
	view->drag3DCB(drag3D);
	view->frameCB(frame);
	view->renderFunc(render);
	win->show();
	_JGL::run();
	return 0;
}

