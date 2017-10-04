#clear octave
close all
clear
clc

#generate/load our map
global map = getMap(10, 10);

#initialize actual robot position x,y (not a state visible to the robot)
global robot_position = [2,2];

#initialize robot states: position belief values over the complete grid
belief_initial_value = 1/(rows(map)*columns(map));
global belief_matrix = ones(rows(map), columns(map))*belief_initial_value;
global observations  = [0 0 0 0];

#function that is executed when a key is pressed (user input)
function keyPressed (src_, event_)

  #available robot controls (corresponding to keyboard key values)
  global MOVE_UP    = 105;
  global MOVE_DOWN  = 107;
  global MOVE_LEFT  = 106;
  global MOVE_RIGHT = 108;
  
  #evaluate which key was pressed
  control_input = 0;
  switch(event_.Key)
    case MOVE_UP
      control_input = MOVE_UP;
      control_input_string = "MOVE UP";
    case MOVE_DOWN
      control_input = MOVE_DOWN;
      control_input_string = "MOVE DOWN";
    case MOVE_LEFT
      control_input = MOVE_LEFT;
      control_input_string = "MOVE LEFT";
    case MOVE_RIGHT
      control_input = MOVE_RIGHT;
      control_input_string = "MOVE RIGHT";
    otherwise
      return;
  endswitch
  fflush(stdout);
  
  #fetch global variables
  global map;
  global robot_position;
  global belief_matrix;
  global observations;
  global subfigure_ground_truth;
  global subfigure_belief;
  map_rows = rows(map);
  map_cols = columns(map);
  
  #erase previous robot position and observations
  subplot(subfigure_ground_truth);
  rectangle("Position", [robot_position(2)-1 robot_position(1)-1 1 1], "FaceColor", "white");
  clearObservations(observations, robot_position, map);
  
#---------------------------------- FILTERING ----------------------------------

  #retrieve new robot position according to our transition model
  robot_position = getRobotPosition(map, robot_position, control_input);
  
  #obtain current observations according to our observation model
  observations = getObservations(map, robot_position);

  #INITIALIZE robot position belief
  belief_matrix_previous = belief_matrix;
  belief_matrix = zeros(map_rows, map_cols);

  #PREDICT robot position belief
	for row = 1:map_rows
		for col = 1:map_cols
      belief_matrix += transitionModel(map, row, col, control_input)*belief_matrix_previous(row, col);
		endfor
	endfor

  #UPDATE robot position belief and COMPUTE the normalizer
	inverse_normalizer = 0;
	for row = 1:map_rows
		for col = 1:map_cols
			belief_matrix(row, col) *= observationModel(map, row, col, observations);
      inverse_normalizer      += belief_matrix(row, col);
		endfor
	endfor	

  #NORMALIZE the belief probabilities to [0, 1]
	normalizer = 1./inverse_normalizer;	
	belief_matrix *= normalizer;

#---------------------------------- FILTERING ----------------------------------

  #draw new robot position and its observations on the map
  subplot(subfigure_ground_truth);
  rectangle("Position", [robot_position(2)-1 robot_position(1)-1 1 1], "FaceColor", "red");
  drawObservations(observations, robot_position);

  #draw belief
  subplot(subfigure_belief);
  drawBelief(belief_matrix, map);

  #status info
  printf("control input: %s, current position: %i %i, observations: %i %i %i %i\n", 
         control_input_string, robot_position(1), robot_position(2),
         observations(1), observations(2), observations(3), observations(4));
endfunction

#function that is executed once the GUI window is closed
function closedGUI (src_, data_)
  printf("GUI closed, terminating\n");
  global is_gui_active
  is_gui_active = false;
endfunction

#create a figure and hook our callback functions to it
figure("name", "grid_orazio",      #figure title
       "deletefcn", @closedGUI,    #function that is called when the GUI is closed
       "keypressfcn", @keyPressed, #function that is called when a key is pressed
       "selected", "on",           #put figure into focus
       "numbertitle", "off");      #remove figure number
font_size = 15;

#initialize robot position belief
global subfigure_belief = subplot(1, 2, 1);
grid on;
axis("square");
set(title("robot position | belief"), "fontsize", font_size);
drawBelief(belief_matrix, map);

#initialize robot position
global subfigure_ground_truth = subplot(1, 2, 2);
grid on;
axis("square");
set(title("robot position | ground truth"), "fontsize", font_size);
drawMap(map);

#draw initial robot position
hold on;
rectangle("Position", [robot_position(2)-1 robot_position(1)-1 1 1], "FaceColor", "red");
hold off;
printf("map loaded!\n");
printf("Move orazio with [I, J, K, L]\n");

#display GUI as long as active
global is_gui_active = true;
while(is_gui_active)
  refresh();  #refresh figures (callback functions are triggered if keys are pressed)
  pause(0.1); #relax the cpu
endwhile
printf("terminated\n");

#clear octave
close all
clear
clc

