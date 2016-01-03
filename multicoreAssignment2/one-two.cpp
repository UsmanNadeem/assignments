void FIRSTPART(){
	for (int i = 0; i < numIterations; ++i) {

		for (int j = 0; j < numObjects; ++j) {
			bodies[j]->Xforce = 0;
			bodies[j]->Yforce = 0;

			for (int k = 0; k < numObjects; ++k) {
				if (j==k) continue;
				// calculate distance
				long double Xdist = bodies[k]->Xposition - bodies[j]->Xposition;
				long double Ydist = bodies[k]->Yposition - bodies[j]->Yposition;
				long double dist = sqrt(Xdist*Xdist+ Ydist*Ydist);

				// skip if collision
				if (dist < COLLISION_DISTANCE) continue;

				// calculate force
				long double force = (G * bodies[k]->mass * bodies[j]->mass) / (dist*dist);  // F = Gm1m2/r2
				bodies[j]->Xforce += force * (Xdist/dist);
				bodies[j]->Yforce += force * (Ydist/dist);
			}

			bodies[j]->setAcceleration();
			bodies[j]->setVelocity();
		}

		for (Body* body:bodies) {
			body->setPosition();
		}
	}

	for (Body* body:bodies) {
		body->printPosition();
	}
}

void SECONDPART(){

	for (int i = 0; i < numIterations; ++i) {

		# pragma omp parallel for
		for (int j = 0; j < numObjects; ++j) {
			// bodies[j]->Xforce = 0;
			// bodies[j]->Yforce = 0;
			long double XsumForces = 0;
			long double YsumForces = 0;
			# pragma omp parallel for reduction (+:XsumForces,YsumForces)
			for (int k = 0; k < numObjects; ++k) {
				if (j == k) continue;
				// calculate distance
				long double Xdist = bodies[k]->Xposition - bodies[j]->Xposition;
				long double Ydist = bodies[k]->Yposition - bodies[j]->Yposition;
				long double dist = sqrt(Xdist*Xdist+ Ydist*Ydist);

				// skip if collision
				if (dist < COLLISION_DISTANCE) continue;

				// calculate force
				long double force = (G * bodies[k]->mass * bodies[j]->mass) / (dist*dist);  // F = Gm1m2/r2
				// bodies[j]->Xforce += force * (Xdist/dist);
				// bodies[j]->Yforce += force * (Ydist/dist);
				XsumForces += force * (Xdist/dist);
				YsumForces += force * (Ydist/dist);
			}
			bodies[j]->Xforce = XsumForces;
			bodies[j]->Yforce = YsumForces;
		}
		
		# pragma omp parallel for
		for (int j = 0; j < numObjects; ++j) {
			bodies[j]->setAcceleration();
			bodies[j]->setVelocity();
			bodies[j]->setPosition();
		}
	}

	for (Body* body:bodies) {
		body->printPosition();
	}
}