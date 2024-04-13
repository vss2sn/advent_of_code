def generate_data(initial_state, disk_length):
    """Generate data using the modified dragon curve process."""
    data = initial_state
    while len(data) < disk_length:
        # Make a copy of the data and reverse it
        b = data[::-1]
        # Replace 0s with 1s and 1s with 0s in the reversed copy
        b = ''.join('1' if c == '0' else '0' for c in b)
        # Append the reversed and modified copy to the original data
        data += '0' + b
    # Return the first disk_length characters of the generated data
    return data[:disk_length]

def calculate_checksum(data):
    """Calculate the checksum for the given data."""
    while len(data) % 2 == 0:
        # Calculate the checksum for the current data
        checksum = ''.join('1' if data[i] == data[i+1] else '0' for i in range(0, len(data), 2))
        data = checksum
    return data

# Initial state from the puzzle input
initial_state = '10111100110001111'
# Disk length to fill
disk_length = 35651584

# Generate the data
generated_data = generate_data(initial_state, disk_length)
# Calculate the checksum for the generated data
checksum = calculate_checksum(generated_data)

print(f"The correct checksum is: {checksum}")
