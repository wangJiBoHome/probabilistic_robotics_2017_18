function map = getMap(rows_, cols_) 

  #generate/load our map
  map = randi([0 0], rows_, cols_);
  map(:, 1) = ones(rows_, 1);
  map(:, cols_) = ones(rows_, 1);
  map(1, :) = ones(1, cols_);
  map(cols_,:) = ones(1, rows_);
  map(4, 1:4) = ones(1,4);
  map(4:8, 8) = ones(5,1);
  map(8, 5:7) = ones(1,3);
  map(8, 3)   = 1;

endfunction

