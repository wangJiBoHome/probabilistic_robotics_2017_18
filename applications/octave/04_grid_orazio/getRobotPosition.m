function new_robot_position = getRobotPosition(map_, robot_position_, control_input_)

  #obtain transition probabilities (covering motions over the complete map)
  transition_probability_matrix = transitionModel(map_, robot_position_(1), robot_position_(2), control_input_);

	#extract a uniform sample between 0 and 100% as minimum probability to move
	minimum_probability = unifrnd(0, 1);

	#available motion range
	min_row = robot_position_(1)-1; #MOVE_DOWN
  max_row = robot_position_(1)+1; #MOVE_UP
	min_col = robot_position_(2)-1; #MOVE_LEFT
  max_col = robot_position_(2)+1; #MOVE_RIGHT

	#over for the available motion range check if probability is higher than the extracted sample
	cumulative_probability = 0;
	for (row = min_row:max_row)
		for (col = min_col:max_col)
			cumulative_probability += transition_probability_matrix(row, col);
			if(cumulative_probability > minimum_probability)
			
			  #return with new position
				new_robot_position = [row, col];
				return;
			endif
		endfor
	endfor
	
	#stick to initial solution if minimum probability could not be achieved
	new_robot_position = robot_position_;
endfunction

