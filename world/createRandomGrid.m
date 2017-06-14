function [ output_args ] = createRandomGrid( sizeX,sizeY,blockSize,lowerBound,upperBound)

    grid(sizeX,sizeY) = 0;

    for i = 1:sizeX
        for j = 1 : sizeY
            r = lowerBound +  (upperBound - lowerBound)*rand(1,1);
            r = 1 - r;
            r = double(floor(r*10)/10);
            grid(i,j) = r;
        end
    end

    grid = double(grid);

    gazeboGrid(1,1) = 0;
    for i = 1: sizeX
        for j = 1 : sizeY
            k = grid(i,j);

            x = (i-1)*blockSize + 1;
            y = (j-1)*blockSize + 1;

            for u = x : blockSize -1 + x
                for v = y : blockSize - 1 + y
                    gazeboGrid(u,v) = k;
                end
            end
        end
        i
    end
    imwrite(gazeboGrid,'lab1.png');

end

