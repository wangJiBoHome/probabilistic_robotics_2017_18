function observations = getObservations(map_, robot_position_)

  #sample minimum probability
	minimum_probability    = unifrnd(0, 1);
	cumulative_probability = 0;

	#convert numbers from 0-15 to binary, i.e. (0,0,0,0) to (1,1,1,1) to have all the
	#possible observation in form (UP, DOWN, LEFT, RIGHT)
	for observation = 0:15
		current_observations    = getBinaryArray(observation);
		current_probability     = observationModel(map_, robot_position_(1), robot_position_(2), current_observations);
		cumulative_probability += current_probability;
		if(cumulative_probability > minimum_probability)
			observations = current_observations;
			return;
		endif
	endfor
	
	#default: no observations
	observations = [0, 0, 0, 0];
endfunction

