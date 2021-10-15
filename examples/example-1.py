
###############################################################################
# IMPORT STATEMENTS ###########################################################
###############################################################################
import math
import numpy as np
from tudatpy.kernel import constants
from tudatpy.kernel import numerical_simulation
from tudatpy.kernel.astro import element_conversion
from tudatpy.kernel.interface import spice_interface
from tudatpy.kernel.numerical_simulation import environment_setup
from tudatpy.kernel.numerical_simulation import propagation_setup
from tudatpy.kernel import __version__

def main():

    # Load spice kernels.
    spice_interface.load_standard_kernels()

    # Set simulation start and end epochs.
    simulation_start_epoch = 0.0
    simulation_end_epoch = constants.JULIAN_DAY

    ###########################################################################
    # CREATE ENVIRONMENT AND VEHICLE ##########################################
    ###########################################################################

    # Create default body settings for "Earth"
    bodies_to_create = ["Earth"]

    # Create default body settings for bodies_to_create, with "Earth"/"J2000" as
    # global frame origin and orientation
    global_frame_origin = "Earth"
    global_frame_orientation = "J2000"
    body_settings = environment_setup.get_default_body_settings(
        bodies_to_create, global_frame_origin, global_frame_orientation )

    # Create system of bodies (in this case only Earth)
    bodies = environment_setup.create_system_of_bodies(body_settings)

    # Add vehicle object to system of bodies
    bodies.create_empty_body("Delfi-C3")

    ###########################################################################
    # CREATE ACCELERATIONS ####################################################
    ###########################################################################

    # Define bodies that are propagated.
    bodies_to_propagate = ["Delfi-C3"]

    # Define central bodies of propagation.
    central_bodies = ["Earth"]

    # Define accelerations acting on Delfi-C3.
    acceleration_settings_delfi_c3 = dict(
        Earth=[propagation_setup.acceleration.point_mass_gravity()]
    )

    acceleration_settings = {"Delfi-C3": acceleration_settings_delfi_c3}

    # Create acceleration models.
    acceleration_models = propagation_setup.create_acceleration_models(
        bodies, acceleration_settings, bodies_to_propagate, central_bodies
    )

    ###########################################################################
    # CREATE PROPAGATION SETTINGS #############################################
    ###########################################################################

    # Set initial conditions for the Asterix satellite that will be
    # propagated in this simulation. The initial conditions are given in
    # Keplerian elements and later on converted to Cartesian elements.
    earth_gravitational_parameter = bodies.get("Earth").gravitational_parameter
    initial_state = element_conversion.keplerian_to_cartesian_elementwise(
        gravitational_parameter=earth_gravitational_parameter,
        semi_major_axis=7500.0e3,
        eccentricity=0.1,
        inclination=np.deg2rad(85.3),
        argument_of_periapsis=np.deg2rad(235.7),
        longitude_of_ascending_node=np.deg2rad(23.4),
        true_anomaly=np.deg2rad(139.87),
    )

    # Create propagation settings.
    termination_condition = propagation_setup.propagator.time_termination( simulation_end_epoch )
    propagator_settings = propagation_setup.propagator.translational(
        central_bodies,
        acceleration_models,
        bodies_to_propagate,
        initial_state,
        termination_condition
    )
    # Create numerical integrator settings.
    fixed_step_size = 10.0
    integrator_settings = propagation_setup.integrator.runge_kutta_4(
        simulation_start_epoch, fixed_step_size
    )

    ###########################################################################
    # PROPAGATE ORBIT #########################################################
    ###########################################################################

    # Create simulation object and propagate dynamics.
    dynamics_simulator = numerical_simulation.SingleArcSimulator(
        bodies, integrator_settings, propagator_settings, True
    )
    states = dynamics_simulator.state_history

    ###########################################################################
    # PRINT INITIAL AND FINAL STATES ##########################################
    ###########################################################################

    print(
        f"""
Single Earth-Orbiting Satellite Example.
The initial position vector of Delfi-C3 is [km]: \n{
        states[simulation_start_epoch][:3] / 1E3} 
The initial velocity vector of Delfi-C3 is [km]: \n{
        states[simulation_start_epoch][3:] / 1E3}
After {simulation_end_epoch} seconds the position vector of Delfi-C3 is [km]: \n{
        states[simulation_end_epoch][:3] / 1E3}
And the velocity vector of Delfi-C3 is [km]: \n{
        states[simulation_start_epoch][3:] / 1E3}
        """
    )

    ###########################################################################
    # SAVE RESULTS ############################################################
    ###########################################################################
    #
    # io.save2txt(
    #     solution=result,
    #     filename="singleSatellitePropagationHistory.dat",
    #     directory="./tutorial_1",
    # )

    # Final statement (not required, though good practice in a __main__).
    return 0


if __name__ == "__main__":
    main()

