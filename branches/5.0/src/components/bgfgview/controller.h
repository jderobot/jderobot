/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#ifndef BGFGVIEW_CONTROLLER_H
#define BGFGVIEW_CONTROLLER_H
#include <vector>
#include <gbxutilacfr/tracer.h>
#include <jderobotutil/observer.h>
#include <tr1/memory>
#include "model.h"


//View and Controller defined here to avoid circular dependencies

namespace bgfgview {
  class View;//forward declaration
  typedef std::tr1::shared_ptr<View> ViewPtr;

  class Controller {
  public:
    Controller(gbxutilacfr::Tracer &tracer, Model &m);
    virtual ~Controller();
    
    void addView(ViewPtr v) throw();
    //void deleteView(ViewPtr v) throw();

    void exit() throw();//FIXME
    
    void setImage(const colorspaces::Image &img) throw() { _model.updateBGModel(img); }
    void setBGModel(CvBGStatModel* newBGModel) throw() { _model.setBGModel(newBGModel); }


    const Model& model() const throw() { return _model; }    
    gbxutilacfr::Tracer& tracer() { return _tracer; };
  private:
    gbxutilacfr::Tracer &_tracer;
    Model &_model;
    std::vector<ViewPtr> _views;
    
    class PImpl;
    const std::auto_ptr<PImpl> pImpl;
  };

  class View: public jderobotutil::Observer{
  public:
    View(Controller &controller) throw()
      : controller(controller) {}
    virtual ~View() throw() {}
    Controller &controller;
  };
} /*namespace*/

#endif /*BGFGVIEW_CONTROLLER_H*/
