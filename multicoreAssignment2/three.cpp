void THIRDPART(){
	for (int i = 0; i < numIterations; ++i) {
		long double minX = numeric_limits<double long>::max();
		long double maxX = numeric_limits<double long>::min();
		long double minY = numeric_limits<double long>::max();
		long double maxY = numeric_limits<double long>::min();

		for (Body* body:bodies) {
			if (body->Xposition < minX) minX = body->Xposition;
			if (body->Yposition < minY) minY = body->Yposition;
			if (body->Xposition > maxX) maxX = body->Xposition;
			if (body->Yposition > maxY) maxY = body->Yposition;
		}

		QuadTree* tree = new QuadTree(minX, minY, max(maxX-minX, maxY-minY));
		// QuadTree* tree = new QuadTree(numeric_limits<double long>::min(), numeric_limits<double long>::min(),\
		 // numeric_limits<double long>::max() - numeric_limits<double long>::min());
		for (Body* body:bodies) {
			body->Xforce = 0;
			body->Yforce = 0;
			tree->insert(body);
		}

		for (Body* body:bodies) {
			tree->setForcesRecursively(body);
		}

		for (Body* body:bodies) {
			body->setAcceleration();
			body->setVelocity();
			body->setPosition();
		}

		delete tree;
	}

	for (Body* body:bodies) {
		body->printPosition();
	}

}