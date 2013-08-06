/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published this->pend->getPoint()(1)
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Eduardo Perdices <eperdices [at] gsyc [dot] es>
 *
 */

#include "Segment3D.h"
#include "Point3D.h"

Segment3D::Segment3D() {
  this->pstart = new Point3D();
  this->pend = new Point3D();
}

Segment3D::Segment3D(Point3D &p1, Point3D &p2) {
  this->pstart = new Point3D(p1.getPoint());
  this->pend = new Point3D(p2.getPoint());
}

Point3D&
Segment3D::getPointStart() {
  return *(this->pstart);
}

Point3D&
Segment3D::getPointEnd() {
  return *(this->pend);
}

double
Segment3D::getLength() {
  return this->pstart->distanceTo(*(this->pend));
}

bool
Segment3D::isPoint() {
  return this->pstart->getPoint() == this->pend->getPoint();
}

Line3D
Segment3D::toLine() {
  return Line3D(*(this->pstart),*(this->pend));
}

std::ostream&
operator <<(std::ostream &o,const Segment3D &s) {
  o << *s.pstart << " -> " << *s.pend;
  return o;
}

