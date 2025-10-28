import pytest
import os
import sys

def pytest_configure(config):
    """Add custom markers for our tests"""
    config.addinivalue_line(
        "markers", "system_test: mark test as a system test (requires built executable)"
    )

@pytest.fixture
def built_executable():
    """Fixture to provide the path to the built executable"""
    from test_system_calc import get_executable_path
    exe_path = get_executable_path()
    
    if not os.path.exists(exe_path):
        pytest.skip(f"Executable not found at {exe_path}. Build the project first.")
    
    return exe_path