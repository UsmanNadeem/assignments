void FOURTHPART(){

	vector<thread*> threads;
	int numThreads = 8;

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

		for (int k = 0; k < numThreads; ++k) {
				threads.push_back(
					new thread ([=](){
						for (int j = k*numObjects/numThreads; j < numObjects && j < (k+1)*numObjects/numThreads; ++j) {
							tree->setForcesRecursively(bodies[j]);
							bodies[j]->setAcceleration();
							bodies[j]->setVelocity();
						}
					})
				);
		}
		for (thread* t : threads) {
			t->join();
			delete t;
		}
		threads.clear();

		for (int k = 0; k < numThreads; ++k) {
				threads.push_back(
					new thread ([=](){
						for (int j = k*numObjects/numThreads; j < numObjects && j < (k+1)*numObjects/numThreads; ++j) {
							bodies[j]->setPosition();
						}
					})
				);
		}
		for (thread* t : threads) {
			t->join();
			delete t;
		}
		threads.clear();
		

		delete tree;
	}

	for (Body* body:bodies) {
		body->printPosition();
	}

}
