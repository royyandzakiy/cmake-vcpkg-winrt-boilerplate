import subprocess
import pytest
import sys
import os
import time

def get_executable_path():
    """Get the path to the built executable based on the platform and build type"""
    base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
    
    if sys.platform == "win32":
        # Try both possible locations
        paths = [
            os.path.join(base_dir, "build", "windows-base", "Debug", "cmake_project_template.exe"),
            os.path.join(base_dir, "build", "windows-base", "cmake_project_template.exe")
        ]
    elif sys.platform == "darwin":
        paths = [os.path.join(base_dir, "build", "macos-base", "Debug", "cmake_project_template")]
    else:  # linux
        paths = [os.path.join(base_dir, "build", "linux-base", "Debug", "cmake_project_template")]
    
    for path in paths:
        if os.path.exists(path):
            return path
    
    return paths[0]  # Return the first path as the expected location

def run_executable_with_timeout(timeout=15):
    """Run the executable and capture output with proper timeout handling"""
    executable = get_executable_path()
    
    if not os.path.exists(executable):
        pytest.skip(f"Executable not found at {executable}. Please build the project first.")
    
    try:
        # Run the executable and capture output
        process = subprocess.Popen(
            [executable], 
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE, 
            text=True,
            bufsize=1,  # Line buffered
            universal_newlines=True
        )
        
        # Collect output for the specified timeout
        start_time = time.time()
        output_lines = []
        
        while time.time() - start_time < timeout:
            # Check if process has finished
            if process.poll() is not None:
                break
                
            # Read available output
            line = process.stdout.readline()
            if line:
                output_lines.append(line.strip())
                # If we have all calculator outputs, we can break early
                calc_outputs = [l for l in output_lines if any(op in l for op in ['add(', 'sub(', 'mul(', 'div(', 'mod('])]
                if len(calc_outputs) >= 5:
                    # We have all calculator outputs, terminate the process
                    process.terminate()
                    try:
                        process.wait(timeout=5)
                    except subprocess.TimeoutExpired:
                        process.kill()
                    break
            
            time.sleep(0.1)  # Small delay to prevent busy waiting
        
        # If process is still running, terminate it
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                process.kill()
        
        # Get any remaining output
        stdout, stderr = process.communicate()
        if stdout:
            output_lines.extend(stdout.splitlines())
        
        return '\n'.join(output_lines), process.returncode
        
    except Exception as e:
        pytest.fail(f"Failed to run executable: {e}")

def test_run_calc_output():
    """Test that run_calc produces the expected output"""
    output, returncode = run_executable_with_timeout()
    
    # Capture the output lines related to calculator
    calc_lines = [line for line in output.split('\n') if any(op in line for op in ['add(', 'sub(', 'mul(', 'div(', 'mod('])]
    
    # Expected calculator outputs
    expected_outputs = [
        "add(10,4): 14",
        "sub(10,4): 6", 
        "mul(10,4): 40",
        "div(10,4): 2",
        "mod(10,4): 2"
    ]
    
    # Check that all expected calculator outputs are present
    for expected in expected_outputs:
        assert any(expected in line for line in calc_lines), f"Expected output '{expected}' not found in:\n" + '\n'.join(calc_lines)

def test_run_calc_output_format():
    """Test that the calculator output has the correct format"""
    output, returncode = run_executable_with_timeout()
    
    # Check output format using regex patterns
    import re
    
    calc_lines = [line for line in output.split('\n') if any(op in line for op in ['add(', 'sub(', 'mul(', 'div(', 'mod('])]
    
    # Pattern: operation(x,y): result
    pattern = r'^(add|sub|mul|div|mod)\(\d+,\d+\): \d+$'
    
    for line in calc_lines:
        # Extract just the calculator part
        if ':' in line:
            # Take only the part before any extra output after the result
            parts = line.split(':')
            if len(parts) >= 2:
                calc_part = parts[0] + ':' + parts[1]
                assert re.match(pattern, calc_part.strip()), f"Output format incorrect: {line}"

def test_run_calc_operations():
    """Test specific calculator operations"""
    output, returncode = run_executable_with_timeout()
    
    # Test specific operations
    assert "add(10,4): 14" in output
    assert "sub(10,4): 6" in output
    assert "mul(10,4): 40" in output  
    assert "div(10,4): 2" in output
    assert "mod(10,4): 2" in output