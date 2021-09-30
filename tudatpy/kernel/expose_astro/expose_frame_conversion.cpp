/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "tudatpy/docstrings.h"

#include "expose_frame_conversion.h"

#include <tudat/astro/reference_frames.h>
#include <tudat/astro/ephemerides/rotationalEphemeris.h>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>

namespace trf = tudat::reference_frames;
namespace te = tudat::ephemerides;

namespace py = pybind11;

namespace tudatpy {
<<<<<<< HEAD
namespace astro {
namespace frame_conversion {

void expose_frame_conversion(py::module &m) {

    py::enum_<trf::AerodynamicsReferenceFrameAngles>(m, "AerodynamicsReferenceFrameAngles" )
            .value("latitude_angle", trf::AerodynamicsReferenceFrameAngles::latitude_angle)
            .value("longitude_angle", trf::AerodynamicsReferenceFrameAngles::longitude_angle)
            .value("heading_angle", trf::AerodynamicsReferenceFrameAngles::heading_angle)
            .value("flight_path_angle", trf::AerodynamicsReferenceFrameAngles::flight_path_angle)
            .value("angle_of_attack", trf::AerodynamicsReferenceFrameAngles::angle_of_attack)
            .value("angle_of_sideslip", trf::AerodynamicsReferenceFrameAngles::angle_of_sideslip)
            .value("bank_angle", trf::AerodynamicsReferenceFrameAngles::bank_angle)
            .export_values( );

    py::enum_<trf::AerodynamicsReferenceFrames>(m, "AerodynamicsReferenceFrames" )
            .value("inertial_frame", trf::AerodynamicsReferenceFrames::inertial_frame)
            .value("corotating_frame", trf::AerodynamicsReferenceFrames::corotating_frame)
            .value("vertical_frame", trf::AerodynamicsReferenceFrames::vertical_frame)
            .value("trajectory_frame", trf::AerodynamicsReferenceFrames::trajectory_frame)
            .value("aerodynamic_frame", trf::AerodynamicsReferenceFrames::aerodynamic_frame)
            .value("body_frame", trf::AerodynamicsReferenceFrames::body_frame)
            .export_values( );

    py::class_<trf::AerodynamicAngleCalculator,
            std::shared_ptr<trf::AerodynamicAngleCalculator>>(m, "AerodynamicAngleCalculator")
            .def("set_orientation_angle_functions",
                 py::overload_cast<
                 const std::function<double()>,
                 const std::function<double()>,
                 const std::function<double()>,
                 const std::function<void(const double)>>(&trf::AerodynamicAngleCalculator::setOrientationAngleFunctions),
                 py::arg("angle_of_attack_function") = std::function<double()>(),       // <pybind11/functional.h>
                 py::arg("angle_of_sideslip_function") = std::function<double()>(),     // <pybind11/functional.h>
                 py::arg("bank_angle_function") = std::function<double()>(),            // <pybind11/functional.h>
                 py::arg("angle_update_function") = std::function<void(const double)>(),// <pybind11/functional.h>
                 "<no_doc>")
            .def("set_orientation_angle_functions",
                 py::overload_cast<
                 const double,
                 const double,
                 const double>(&trf::AerodynamicAngleCalculator::setOrientationAngleFunctions),
                 py::arg("angle_of_attack") = TUDAT_NAN,
                 py::arg("angle_of_sideslip") = TUDAT_NAN,
                 py::arg("bank_angle") = TUDAT_NAN,
                 "<no_doc>")
            .def("get_rotation_quaternion_between_frames",
                 &trf::AerodynamicAngleCalculator::getRotationQuaternionBetweenFrames,
                 py::arg("original_frame"),
                 py::arg("target_frame" ) )
            .def("get_rotation_matrix_between_frames",
                 &trf::AerodynamicAngleCalculator::getRotationMatrixBetweenFrames,
                 py::arg("original_frame"),
                 py::arg("target_frame" ) )
            .def("get_angle",
                 &trf::AerodynamicAngleCalculator::getAerodynamicAngle,
                 py::arg("angle_type") );


    m.def("inertial_to_rsw_rotation_matrix",
          &trf::getInertialToRswSatelliteCenteredFrameRotationMatrix,
          py::arg("inertial_cartesian_state"),
          get_docstring("inertial_to_rsw_rotation_matrix").c_str() );

    m.def("rsw_to_inertial_rotation_matrix",
          &trf::getRswSatelliteCenteredToInertialFrameRotationMatrix,
          py::arg("inertial_cartesian_state"),
          get_docstring("rsw_to_inertial_rotation_matrix").c_str() );

    m.def("tnw_to_inertial_rotation_matrix",
          &trf::getTnwToInertialRotation,
          py::arg("inertial_cartesian_state"),
          py::arg("n_axis_points_away_from_central_body" ) = true,
          get_docstring("tnw_to_inertial_rotation_matrix").c_str() );

    m.def("inertial_to_tnw_rotation_matrix",
          &trf::getInertialToTnwRotation,
          py::arg("inertial_cartesian_state"),
          py::arg("n_axis_points_away_from_central_body" ) = true,
          get_docstring("inertial_to_tnw_rotation_matrix").c_str() );


    m.def("inertial_to_body_fixed_rotation_matrix",
         py::overload_cast< const double, const double, const double >(
              &trf::getInertialToPlanetocentricFrameTransformationMatrix ),
          py::arg("pole_declination"),
          py::arg("pole_right_ascension"),
          py::arg("prime_meridian_longitude") );

    m.def("body_fixed_to_inertial_rotation_matrix",
          py::overload_cast< const double, const double, const double >(
              &trf::getRotatingPlanetocentricToInertialFrameTransformationMatrix ),
          py::arg("pole_declination"),
          py::arg("pole_right_ascension"),
          py::arg("pole_meridian") );


    m.def("transform_state_from_inertial_to_body_fixed",
          py::overload_cast< const Eigen::Vector6d&, const Eigen::Matrix3d&, const Eigen::Matrix3d& >(
              &te::transformStateToFrameFromRotations< double > ),
          py::arg("inertial_state"),
          py::arg("rotation_to_body_fixed"),
          py::arg("rotation_derivative_to_body_fixed") );

    m.def("transform_to_inertial_orientation",
          &te::transformStateToInertialOrientation<double, double>,
          py::arg("state_in_body_fixed_frame"),
          py::arg("current_time"),
          py::arg("rotational_ephemeris"));


}
}
}

=======
    namespace astro {
        namespace frame_conversion {

            void expose_frame_conversion(py::module &m) {

                py::enum_<trf::AerodynamicsReferenceFrameAngles>(m, "AerodynamicsReferenceFrameAngles")
                        .value("latitude_angle", trf::AerodynamicsReferenceFrameAngles::latitude_angle)
                        .value("longitude_angle", trf::AerodynamicsReferenceFrameAngles::longitude_angle)
                        .value("heading_angle", trf::AerodynamicsReferenceFrameAngles::heading_angle)
                        .value("flight_path_angle", trf::AerodynamicsReferenceFrameAngles::flight_path_angle)
                        .value("angle_of_attack", trf::AerodynamicsReferenceFrameAngles::angle_of_attack)
                        .value("angle_of_sideslip", trf::AerodynamicsReferenceFrameAngles::angle_of_sideslip)
                        .value("bank_angle", trf::AerodynamicsReferenceFrameAngles::bank_angle)
                        .export_values();

                py::enum_<trf::AerodynamicsReferenceFrames>(m, "AerodynamicsReferenceFrames")
                        .value("inertial_frame", trf::AerodynamicsReferenceFrames::inertial_frame)
                        .value("corotating_frame", trf::AerodynamicsReferenceFrames::corotating_frame)
                        .value("vertical_frame", trf::AerodynamicsReferenceFrames::vertical_frame)
                        .value("trajectory_frame", trf::AerodynamicsReferenceFrames::trajectory_frame)
                        .value("aerodynamic_frame", trf::AerodynamicsReferenceFrames::aerodynamic_frame)
                        .value("body_frame", trf::AerodynamicsReferenceFrames::body_frame)
                        .export_values();

                py::class_<trf::AerodynamicAngleCalculator,
                        std::shared_ptr<trf::AerodynamicAngleCalculator>>(m, "AerodynamicAngleCalculator")
                        .def("set_orientation_angle_functions",
                             py::overload_cast<
                                     const std::function<double()>,
                                     const std::function<double()>,
                                     const std::function<double()>,
                                     const std::function<void(const double)>>(
                                     &trf::AerodynamicAngleCalculator::setOrientationAngleFunctions),
                             py::arg("angle_of_attack_function") = std::function<double()>(),       // <pybind11/functional.h>
                             py::arg("angle_of_sideslip_function") = std::function<double()>(),     // <pybind11/functional.h>
                             py::arg("bank_angle_function") = std::function<double()>(),            // <pybind11/functional.h>
                             py::arg("angle_update_function") = std::function<void(
                                     const double)>(),// <pybind11/functional.h>
                             "<no_doc>")
                        .def("set_orientation_angle_functions",
                             py::overload_cast<
                                     const double,
                                     const double,
                                     const double>(&trf::AerodynamicAngleCalculator::setOrientationAngleFunctions),
                             py::arg("angle_of_attack") = TUDAT_NAN,
                             py::arg("angle_of_sideslip") = TUDAT_NAN,
                             py::arg("bank_angle") = TUDAT_NAN,
                             "<no_doc>")
                        .def("get_rotation_quaternion_between_frames",
                             &trf::AerodynamicAngleCalculator::getRotationQuaternionBetweenFrames,
                             py::arg("original_frame"),
                             py::arg("target_frame"))
                        .def("get_rotation_matrix_between_frames",
                             &trf::AerodynamicAngleCalculator::getRotationMatrixBetweenFrames,
                             py::arg("original_frame"),
                             py::arg("target_frame"))
                        .def("get_angle",
                             &trf::AerodynamicAngleCalculator::getAerodynamicAngle,
                             py::arg("angle_type"));


                m.def("inertial_to_rsw_rotation_matrix",
                      &trf::getInertialToRswSatelliteCenteredFrameRotationMatrix,
                      py::arg("inertial_cartesian_state"),
                      get_docstring("inertial_to_rsw_rotation_matrix").c_str());

                m.def("rsw_to_inertial_rotation_matrix",
                      &trf::getRswSatelliteCenteredToInertialFrameRotationMatrix,
                      py::arg("inertial_cartesian_state"),
                      get_docstring("rsw_to_inertial_rotation_matrix").c_str());

                m.def("tnw_to_inertial_rotation_matrix",
                      &trf::getTnwToInertialRotation,
                      py::arg("inertial_cartesian_state"),
                      py::arg("n_axis_points_away_from_central_body") = true,
                      get_docstring("tnw_to_inertial_rotation_matrix").c_str());

                m.def("inertial_to_tnw_rotation_matrix",
                      &trf::getInertialToTnwRotation,
                      py::arg("inertial_cartesian_state"),
                      py::arg("n_axis_points_away_from_central_body") = true,
                      get_docstring("inertial_to_tnw_rotation_matrix").c_str());


                m.def("inertial_to_body_fixed_rotation_matrix",
                      py::overload_cast<const double, const double, const double>(
                              &trf::getInertialToPlanetocentricFrameTransformationMatrix),
                      py::arg("pole_declination"),
                      py::arg("pole_right_ascension"),
                      py::arg("prime_meridian_longitude"));

                m.def("body_fixed_to_inertial_rotation_matrix",
                      py::overload_cast<const double, const double, const double>(
                              &trf::getRotatingPlanetocentricToInertialFrameTransformationMatrix),
                      py::arg("pole_declination"),
                      py::arg("pole_right_ascension"),
                      py::arg("pole_meridian"));


                m.def("transform_state_from_inertial_to_body_fixed",
                      py::overload_cast<const Eigen::Vector6d &, const Eigen::Matrix3d &, const Eigen::Matrix3d &>(
                              &te::transformStateToFrameFromRotations<double>),
                      py::arg("inertial_state"),
                      py::arg("rotation_to_body_fixed"),
                      py::arg("rotation_derivative_to_body_fixed"));


            }

        } // namespace frame_conversion
    } // namespace astro
>>>>>>> 430786d34e5d83dfba61cf73c209ba1eca6b921e
}// namespace tudatpy
