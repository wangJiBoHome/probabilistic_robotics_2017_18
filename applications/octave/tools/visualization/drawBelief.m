function drawBelief(belief_matrix_, map_)
	
	#invert belief value for plotting (0:black: 100% confidence, 1:white: 0% confidence)
	plotted_belief_matrix_ = ones(size(belief_matrix_)) - belief_matrix_;

  #plot a colormap with the respective belief values
	colormap(gray(64));
	hold on;
	image([0.5, columns(map_)-0.5], [0.5, rows(map_)-0.5], plotted_belief_matrix_*64);
	hold off;
endfunction

