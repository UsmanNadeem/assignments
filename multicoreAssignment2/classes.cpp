
class Body {
public:
	long double Xvelocity,Yvelocity,Xposition,Yposition, mass, Xforce, Yforce, Xacceleration, Yacceleration;
	Body(long double vx, long double vy, long double px, long double py, long double m)  {
		Xvelocity = vx;
		Yvelocity = vy;
		Xposition = px;
		Yposition = py;
		mass = m;
		Xforce = 0;
		Yforce = 0;
		Xacceleration = 0;
		Yacceleration = 0;
	};

	void setAcceleration() {  // a = F/m  using const force
		Xacceleration = Xforce / mass;
		Yacceleration = Yforce / mass;
	}
	void setVelocity() {  // v = v0+at  using constant Acc
		Xvelocity += Xacceleration * dt;
		Yvelocity += Yacceleration * dt;
	}
	void setPosition() {  // s = vt  using constant velocity
		Xposition += Xvelocity * dt;
		Yposition += Yvelocity * dt;
	}
	void printPosition() {
		// printf("Xposition %Lf Yposition %Lf\n", Xposition, Yposition);
		printf("%Lf, %Lf\n", Xposition, Yposition);
		// printf("%Lf, %Lf\n", Xvelocity, Yvelocity);
		// printf("%Lf, %Lf\n", Xforce, Yforce);
		// printf("%Lf, %Lf\n", Xacceleration, Yacceleration);
	}
};





class QuadTree {
public:
	long double Xposition, Yposition, length;

	Body* body;
	QuadTree* NE;
	QuadTree* NW;
	QuadTree* SE;
	QuadTree* SW;
	
	QuadTree(long double xp, long double yp, long double l) {
		body = NULL;
		NE = NULL;
		NW = NULL;
		SE = NULL;
		SW = NULL;
		Xposition = xp;
		Yposition = yp;
		length = l;
	}
	~QuadTree() {
		if (NW == NULL && NE == NULL && SW == NULL && SE == NULL) return;
		if (NW) delete NW;
		if (SW) delete SW;
		if (SE) delete SE;
		if (NE) delete NE;
		delete body;

	}
	int getQuadrant(Body* b) {
		long double x = b->Xposition;
		long double y = b->Yposition;
		if (x< Xposition + 0.5*length) {
			if (y < Yposition + 0.5*length) {
				return 4;
			} else {
				return 1;
			}
		} else {
			if (y < Yposition + 0.5*length) {
				return 3;
			} else {
				return 2;
			}
		}
	}
	void insert(Body* b) {

		if(NW == NULL && NE == NULL && SW == NULL && SE == NULL) {  // external
			// external with no body
			if (body == NULL) {
				body = b;
				return;
			}

			// push the old one one step down ... preserve pointer value
			int quadrant = getQuadrant(body);
			if (quadrant == 1) {
				NW = new QuadTree(Xposition, Yposition+(length/2), length/2);
				NW->insert(body);
			} else if (quadrant == 2) {
				NE = new QuadTree(Xposition+(length/2), Yposition+(length/2), length/2);
				NE->insert(body);
			} else if (quadrant == 3) {
				SE = new QuadTree(Xposition+(length/2), Yposition, length/2);
				SE->insert(body);
			} else if (quadrant == 4) {
				SW = new QuadTree(Xposition, Yposition, length/2);
				SW->insert(body);
			}

			// combine two bodies
			// x = m1x1 + m2x2 / m1+m2
			long double x = (body->mass*body->Xposition + b->mass*b->Xposition) / (body->mass + b->mass);
			long double y = (body->mass*body->Yposition + b->mass*b->Yposition) / (body->mass + b->mass);
			long double m = body->mass + b->mass;
			body = new Body(0, 0, x, y, m);

			// insert the new one again.
			insert(b);
		} else {  // internal node => update center of mass and put it down the tree
			// calculate appropriate quadrant and recursively add it there ... check for null quadrant
			int quadrant = getQuadrant(b);
			if (quadrant == 1) {
				if (!NW) NW = new QuadTree(Xposition, Yposition+(length/2), length/2);
				NW->insert(b);
			} else if (quadrant == 2) {
				if (!NE) NE = new QuadTree(Xposition+(length/2), Yposition+(length/2), length/2);
				NE->insert(b);
			} else if (quadrant == 3) {
				if (!SE) SE = new QuadTree(Xposition+(length/2), Yposition, length/2);
				SE->insert(b);
			} else if (quadrant == 4) {
				if (!SW) SW = new QuadTree(Xposition, Yposition, length/2);
				SW->insert(b);
			}

			// combine two bodies
			// x = m1x1 + m2x2 / m1+m2
			long double x = (body->mass*body->Xposition + b->mass*b->Xposition) / (body->mass + b->mass);
			long double y = (body->mass*body->Yposition + b->mass*b->Yposition) / (body->mass + b->mass);
			long double m = body->mass + b->mass;
			delete body;
			body = new Body(0, 0, x, y, m);
		}
	}


	void setForcesRecursively(Body* b) {
		if (body == NULL) return;
		
		// calculate distance
		long double Xdist = body->Xposition - b->Xposition;
		long double Ydist = body->Yposition - b->Yposition;
		long double dist = sqrt(Xdist*Xdist+ Ydist*Ydist);

		if (NW == NULL && NE == NULL && SW == NULL && SE == NULL) {  // last level external node
			if (body == b) return;

			// skip if collision 
			if (dist < COLLISION_DISTANCE) return;

			// calculate force
			long double force = (G * body->mass * b->mass) / (dist*dist);  // F = Gm1m2/r2
			b->Xforce += (force * (Xdist/dist));
			b->Yforce += (force * (Ydist/dist));
		}

		// width of the region represented by the node / distance b/w body & center of mass of node
		else if (length / dist < errorThreshold){
			// skip if collision 
			if (dist < COLLISION_DISTANCE) return;

			// calculate force
			long double force = (G * body->mass * b->mass) / (dist*dist);  // F = Gm1m2/r2
			b->Xforce += (force * (Xdist/dist));
			b->Yforce += (force * (Ydist/dist));

		} else {
			// cout << length / dist << endl;
			if (NE) NE->setForcesRecursively(b);
			if (NW) NW->setForcesRecursively(b);
			if (SE) SE->setForcesRecursively(b);
			if (SW) SW->setForcesRecursively(b);
		}
	}
};

