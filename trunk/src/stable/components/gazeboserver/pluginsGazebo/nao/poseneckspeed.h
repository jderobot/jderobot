/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include <boost/bind.hpp>
#include "gazebo.hh"
#include "physics/physics.hh"
#include "common/common.hh"
#include "transport/transport.hh"

// ICE utils includes
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

// JDErobot general ice component includes
#include <jderobot/pose3dencoders.h>
#include <jderobot/pose3dmotors.h>

#include "common.h"

#ifndef NECKSPEED_H
#define NECKSPEED_H

namespace gazebo {

    void* thread_NeckSpeedICE ( void* v );

    class PoseNeckSpeed : public ModelPlugin {
    public:

        PoseNeckSpeed ();
        virtual void Load ( physics::ModelPtr _model, sdf::ElementPtr _sdf );
        virtual void Init ();

        std::string cfgfile_neckspeed;
        pthread_mutex_t mutex_neckspeedencoders;
        pthread_mutex_t mutex_neckspeedmotors;

        struct neckspeed_t {
            physics::JointPtr joint_tilt, joint_pan;
            physics::LinkPtr link_pan, link_tilt;
            encoders_t encoders;
            motorsdata_t motorsdata;
            motorsparams_t motorsparams;
        };

        neckspeed_t neckspeed;

    private:
        void OnUpdate ();
        
        event::ConnectionPtr updateConnection;
        double stiffness;
        int count;
        int cycle;
    };
}

#endif // NECKSPEED_H
