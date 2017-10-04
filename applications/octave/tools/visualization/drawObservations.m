function drawObservations(observations_, robot_position_)
  row = robot_position_(1)-1;
  col = robot_position_(2)-1;
	hold on;
	
	#check the observation array
	if (observations_(1)) #UP
    rectangle("Position", [col row+1 1 0.5], "FaceColor", "blue", "EdgeColor", "none");
	endif
	if (observations_(2)) #DOWN
    rectangle("Position", [col row-0.5 1 0.5], "FaceColor", "blue", "EdgeColor", "none");
	endif
	if (observations_(3)) #LEFT
    rectangle("Position", [col-0.5 row 0.5 1], "FaceColor", "blue", "EdgeColor", "none");
	endif
	if (observations_(4)) #RIGHT
    rectangle("Position", [col+1 row 0.5 1], "FaceColor", "blue", "EdgeColor", "none");
	endif
	
	hold off;
endfunction

