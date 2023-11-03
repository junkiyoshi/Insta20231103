#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetColor(148);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font_size = 120;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->base_location_list.clear();

	this->word = "HBD 39";
	auto path_list = font.getStringAsPoints(this->word, true, false);

	for (auto& path : path_list) {

		vector<ofPolyline> outline = path.getOutline();

		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(10);
			auto vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto location = glm::vec3(vertices[vertices_index].x - this->font.stringWidth(this->word) * 0.5, vertices[vertices_index].y + this->font.stringHeight(this->word) * 0.25, 0);
				this->base_location_list.push_back(location);
			}
		}
	}

	this->log_list.clear();

	int limit = 500;
	for (int i = 0; i < this->base_location_list.size(); i++) {

		vector<glm::vec2> log;
		log.push_back(glm::vec2(this->base_location_list[i]));
		this->log_list.push_back(log);
	}

	int step = 3;
	for (int i = 0; i < this->log_list.size(); i++) {

		int k = 0;
		while (true) {

			auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.0045, ofGetFrameNum() * 0.003 + k * 0.0002)), 0, 1, -360, 360);
			auto next = this->log_list[i].back() + glm::vec2(step * cos(deg * DEG_TO_RAD), step * sin(deg * DEG_TO_RAD));

			if (next.x < -limit || next.x > limit || next.y < -limit || next.y > limit) {

				break;
			}
			else {

				this->log_list[i].push_back(next);
			}

			k++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofColor color;
	ofSetLineWidth(1);
	for (int i = 0; i < this->log_list.size(); i++) {

		color.setHsb(ofMap(i % 10, 0, 10, 0, 255), 255, 255);
		ofSetColor(color, 150);

		if (this->log_list[i].size() > 1) {

			ofNoFill();
			ofBeginShape();
			ofVertices(this->log_list[i]);
			ofEndShape();
		}
	}

	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255);
	this->font.drawStringAsShapes(this->word, this->font.stringWidth(this->word) * -0.5, this->font.stringHeight(this->word) * 0.25);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}