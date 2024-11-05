#pragma once

class ofxKanaKeyboard{
public:
	ofxKanaKeyboard(){
		windowWidth = ofGetWidth();
		windowHeight = ofGetHeight();

		gridWidth = 6.0;

		for (int c = 0; c < columnSize; c++) {
			for (int r = 0; r < rowSize; r++) {
				keyPressedTimeStart[r][c] = 0.0;
				keyPressedTimeEnd[r][c] = 0.0;
				keyPressedColor[r][c].set(128, 243, 228);
				keyPressed[r][c] = false;
			}
		}

		for (int c = 0; c < specialKeySize; c++) {
			specialkeyPressedTimeStart[c] = 0.0;
			specialkeyPressedColor[c].set(51, 51, 51);
			specialkeyPressed[c] = false;
		}
	}

    void setup(string font, int fontSize){
		ofTrueTypeFontSettings fontSettings(font, fontSize);
		fontSettings.antialiased = true;
		fontSettings.dpi = 72;
		fontSettings.direction = OF_TTF_LEFT_TO_RIGHT;
		fontSettings.addRanges(ofAlphabet::Japanese);

		ttf.load(fontSettings);

		keyRectWidth = ((windowWidth - gridWidth * (columnSize + 1)) / columnSize) ;
		keyRectHeight = keyRectWidth;

		for (int c = 0; c < columnSize; c++) {
			for (int r = 0; r < rowSize; r++) {
				keyMatrixPos[r][c].x = windowWidth - ((gridWidth + keyRectWidth) * (c + 1));
				keyMatrixPos[r][c].y = (gridWidth + keyRectHeight) * r + (windowHeight - ((gridWidth + keyRectHeight) * (rowSize + 1)));
				keyRect[r][c].set(keyMatrixPos[r][c], keyRectWidth, keyRectHeight);
			}
		}
		
		int specialKeyPos = 0;
		for (int c = 0; c < specialKeySize; c++) {
			specialKeyPos += specialKeyRange[c];
			specialKeyMatrixPos[c].x = windowWidth - (gridWidth * specialKeyPos + keyRectWidth * specialKeyPos);
			specialKeyMatrixPos[c].y = windowHeight - gridWidth - keyRectHeight;
			specialKeyRect[c].set(specialKeyMatrixPos[c], gridWidth * (specialKeyRange[c] - 1) + keyRectWidth * specialKeyRange[c], keyRectHeight);
		}
	}

	void update(float now) {
		for (int c = 0; c < columnSize; c++) {
			for (int r = 0; r < rowSize; r++) {
				if (keyPressed[r][c]) {
					keyPressedColor[r][c].r = ofMap(now, keyPressedTimeStart[r][c], keyPressedTimeEnd[r][c], 250, 128);
					keyPressedColor[r][c].g = ofMap(now, keyPressedTimeStart[r][c], keyPressedTimeEnd[r][c], 250, 243);
					keyPressedColor[r][c].b = ofMap(now, keyPressedTimeStart[r][c], keyPressedTimeEnd[r][c], 250, 228);
					if (now >= keyPressedTimeEnd[r][c]) {
						keyPressed[r][c] = false;
					}
				}
			}
		}
		for (int c = 0; c < specialKeySize; c++) {
			if (specialkeyPressed[c]) {
				specialkeyPressedColor[c].r = ofMap(now, specialkeyPressedTimeStart[c], specialkeyPressedTimeEnd[c], 201, 51);
				specialkeyPressedColor[c].g = ofMap(now, specialkeyPressedTimeStart[c], specialkeyPressedTimeEnd[c], 201, 51);
				specialkeyPressedColor[c].b = ofMap(now, specialkeyPressedTimeStart[c], specialkeyPressedTimeEnd[c], 201, 51);
				if (now >= specialkeyPressedTimeEnd[c]) {
					specialkeyPressed[c] = false;
				}
			}
		}
	}
    
	void draw() {
		if (display) {
			for (int c = 0; c < columnSize; c++) {
				for (int r = 0; r < rowSize; r++) {
					ofPushStyle();

					ofSetColor(keyPressedColor[r][c]);
					ofDrawRectangle(keyRect[r][c]);
					ofSetColor(255);
					ofPushMatrix();
					int index = (r + (rowSize * c)) * 3;
					string key = keymode->substr(index, 3);
					ofRectangle BoundingBox = ttf.getStringBoundingBox(key, keyRect[r][c].getTopLeft().x, keyRect[r][c].getTopLeft().y);
					float tX = keyRect[r][c].getCenter().x - BoundingBox.getCenter().x;
					float tY = keyRect[r][c].getCenter().y - BoundingBox.getCenter().y;
					ofTranslate(tX, tY);
					ttf.drawString(key, keyRect[r][c].getTopLeft().x, keyRect[r][c].getTopLeft().y);
					ofPopMatrix();

					ofPopStyle();
				}
			}
			for (int c = 0; c < specialKeySize; c++) {
				ofPushStyle();

				ofSetColor(specialkeyPressedColor[c]);
				ofDrawRectangle(specialKeyRect[c]);
				ofSetColor(255);
				ofPushMatrix();
				ofRectangle BoundingBox = ttf.getStringBoundingBox(specialKeyLabel[c], specialKeyRect[c].x, specialKeyRect[c].y);
				float tX = specialKeyRect[c].getCenter().x - BoundingBox.getCenter().x;
				float tY = specialKeyRect[c].getCenter().y - BoundingBox.getCenter().y;
				ofTranslate(tX, tY);
				ttf.drawString(specialKeyLabel[c], (float)specialKeyRect[c].getTopLeft().x, (float)specialKeyRect[c].getTopLeft().y);
				ofPopMatrix();

				ofPopStyle();
			}
		}
    }

	void drawSentence() {
		ofPushStyle();
		ofSetColor(0);
		ttf.drawString(strSentence, 100, 100);
		ofPopStyle();
	}

	void setPressed(int x, int y) {
		if (!display) {
			return;
		}
		glm::vec2 v(x, y);
		clickPos = v;

		for (int c = 0; c < columnSize; c++) {
			for (int r = 0; r < rowSize; r++) {
				if (keyRect[r][c].inside(clickPos)) {
					keyPressedTimeStart[r][c] = ofGetElapsedTimef();
					keyPressedTimeEnd[r][c] = keyPressedTimeStart[r][c] + 0.5;
					keyPressed[r][c] = true;
					int index = (r + (rowSize * c)) * 3;
					strSentence.append(keymode->substr(index, 3));
				}
			}
		}
		for (int c = 0; c < specialKeySize; c++) {
			if (specialKeyRect[c].inside(clickPos)) {
				specialkeyPressedTimeStart[c] = ofGetElapsedTimef();
				specialkeyPressedTimeEnd[c] = specialkeyPressedTimeStart[c] + 0.5;
				specialkeyPressed[c] = true;

				if (c == 0) {
					strSentence.append("\n");
					break;
				}
				else if (c == 1) {
					if (strSentence.empty()) {
						break;
					}
					int range = 3;
					if (strSentence.substr(strSentence.size() - 1, 1) == "\n") {
						range = 1;
					}
					strSentence.erase(strSentence.size() - range, strSentence.size());
					break;
				}
				else if (c == 2) {
					strSentence.append(u8"　");
					break;
				}
				else if (c == 3) {
					break;
				}
				else if (c == 4) {
					if (keymode == &katakana) {
						keymode = &hiragana;
						specialKeyLabel[4] = u8"カナ";
						break;
					}
					else {
						keymode = &katakana;
						specialKeyLabel[4] = u8"かな";
						break;
					}
				}
			}
		}
	}

	void toggle() {
		display = !display;
	}

private:
	int windowWidth;
	int windowHeight;

	ofTrueTypeFont ttf;
	string hiragana = u8"あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよー・らりるれろわゐゑをんがぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽぁぃぅぇぉっゃゅょゞ";
	string katakana = u8"アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨー・ラリルレロワヰヱヲンガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポァィゥェォッャュョヴ";

	string* keymode = &katakana;

	static const int rowSize = 5;
	static const int columnSize = 17;

	glm::vec2 keyMatrixPos[rowSize][columnSize];
    ofRectangle keyRect[rowSize][columnSize];

	ofColor keyPressedColor[rowSize][columnSize];
	float keyPressedTimeStart[rowSize][columnSize];
	float keyPressedTimeEnd[rowSize][columnSize];
	bool keyPressed[rowSize][columnSize];


	static const int specialKeySize = 5;
	int specialKeyRange[specialKeySize] = {2, 2, 9, 2, 2};
	string specialKeyLabel[specialKeySize] = { u8"改行", u8"削除", "", u8"数字", u8"かな" };

	ofRectangle specialKeyRect[specialKeySize];
	glm::vec2 specialKeyMatrixPos[specialKeySize];

	ofColor specialkeyPressedColor[specialKeySize];
	float specialkeyPressedTimeStart[specialKeySize];
	float specialkeyPressedTimeEnd[specialKeySize];
	bool specialkeyPressed[specialKeySize];

	float keyRectWidth;
	float keyRectHeight;
	float gridWidth;

	glm::vec2 clickPos;

	string strSentence;

	bool display = true;
};