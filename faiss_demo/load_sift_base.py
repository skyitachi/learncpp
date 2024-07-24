import struct
import numpy as np

def read_fvecs(filename):
    with open(filename, 'rb') as f:
        dimension = struct.unpack('i', f.read(4))[0]
        f.seek(0)
        data = np.fromfile(f, dtype=np.float32)

    # Reshape the data to have the correct number of dimensions
    data = data.reshape(-1, dimension + 1)

    # Extract the vectors by removing the first column (which contains the dimension)
    vectors = data[:, 1:].copy()

    return vectors

# Example usage
filename = '/home/skyitachi/lab/learnpython/data/sift/sift_base.fvecs'

vectors = read_fvecs(filename)
print(vectors.shape)  # Output the shape of the vectors
print(vectors[:5])