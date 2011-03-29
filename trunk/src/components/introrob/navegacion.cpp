/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Author : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "navegacion.h"

namespace introrob {
	int Navegacion::pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color) {
		glColor3f(color.x, color.y, color.z);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			v3f(a.x, a.y, a.z);
			v3f(b.x, b.y, b.z);
		glEnd();
		return 1;
	}

	int Navegacion::absolutas2relativas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL) {
			(*out).x = in.x*cos(this->robottheta*DEGTORAD) - in.y*sin(this->robottheta*DEGTORAD) +
				this->robotx*cos(this->robottheta*DEGTORAD) + this->roboty*sin(this->robottheta*DEGTORAD);
			(*out).y = in.y*cos(this->robottheta*DEGTORAD) + in.x*sin(this->robottheta*DEGTORAD) +
				this->roboty*sin(this->robottheta*DEGTORAD) + this->roboty*cos(this->robottheta*DEGTORAD);
			return 0;
		}
		return 1;
	}

	int Navegacion::relativas2absolutas(CvPoint3D32f in, CvPoint3D32f *out)	{
		if (out!=NULL){
			(*out).x = in.x*cos(this->robottheta*DEGTORAD) - in.y*sin(this->robottheta*DEGTORAD) + this->robotx;
			(*out).y = in.y*cos(this->robottheta*DEGTORAD) + in.x*sin(this->robottheta*DEGTORAD) + this->roboty;
			return 0;
		}
		return 1;
	}

	void* callback(void* obj) {
		static_cast<Navegacion*>(obj)->main();
		return(0);
	} // callback

	void Navegacion::cogerImagen1(unsigned char** image) { // refresco el contenido de la imagen1
		this->controller->getCameraData1 (&myImage1);
		*image = &myImage1[0];
	}

	void Navegacion::cogerImagen2(unsigned char** image) { // refresco el contenido de la imagen1
		this->controller->getCameraData2 (&myImage2);
		*image = &myImage2[0];
	}

	void Navegacion::cogerPosicion(CvPoint3D32f* myPoint) { // refresco la posición del pioneer
		this->controller->getPosition (myPoint);
	}

	int Navegacion::cogerLaser(std::vector<float>* laser) { // refrescamos el vector de valores de láser
		this->controller->getLaser (laser);
		return this->controller->getNumLasers();
	}

	void Navegacion::cogerDestino(CvPoint2D32f* destino) { // refresco la posición del pioneer
		printf ("navegacion antes\n");
		this->view->setDestino ();
		destino->x = this->view->destino.x;
		destino->y = this->view->destino.y;
		printf ("navegacion destino = %f, %f\n", destino->x, destino->y);
	}

	int Navegacion::main() {
		struct timeval a, b;
		int cycle = 100;
		long totalb,totala;
		long diff;

		while(1) {
			gettimeofday(&a,NULL);
			totala=a.tv_sec*1000000+a.tv_usec;

			if (this->running) {
				this->navega->iteracionControl();
			}

			gettimeofday(&b,NULL);
			totalb=b.tv_sec*1000000+b.tv_usec;
			//std::cout << "Introrob takes " << (totalb-totala)/1000 << " ms" << std::endl;

			diff = (totalb-totala)/1000;
			if(diff < 0 || diff > cycle)
				diff = cycle;
			else
				diff = cycle-diff;

			/*Sleep Algorithm*/
			usleep(diff*1000);
			if(diff < 33)
				usleep(33*1000);
		}
	}

	void Navegacion::run(introrob::Controller * controller) {
		this->controller = controller;
		this->navega = new Navega (this->controller, this);
		this->running=false;
		this->myImage1 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));
		this->myImage2 = (unsigned char*) calloc (this->controller->data1->description->width*this->controller->data1->description->height*3,sizeof(unsigned char));

    pthread_create(&thread, 0, &callback, this);
  }

	void Navegacion::start() {
    this->running=true;
  }

	void Navegacion::stop() {
		this->controller->stopMotors();
    this->running=false;
  }

  int Navegacion::join() {
    return pthread_join(thread, ret);
  }
} // namespace
