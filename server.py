#!/usr/bin/env python3
"""
Backend server to connect the web interface to the C calculator program.
"""

from flask import Flask, request, jsonify, send_file
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

# Path to the calculator executable
CALCULATOR_PATH = './calculator_single'

@app.route('/')
def home():
    """Serve the interface.html file"""
    return send_file('interface.html', mimetype='text/html')

@app.route('/calculate', methods=['POST'])
def calculate():
    try:
        data = request.json
        expression = data.get('expression', '')
        x_value = data.get('xValue', 10)
        
        if not expression:
            return jsonify({'error': 'Expression is required'}), 400
        
        # Run the calculator program
        result = subprocess.run(
            [CALCULATOR_PATH, expression, str(x_value)],
            capture_output=True,
            text=True,
            timeout=5
        )
        
        if result.returncode != 0:
            return jsonify({'error': 'Calculation failed', 'details': result.stderr}), 500
        
        # Parse the output
        output = result.stdout
        parsed = parse_output(output)
        
        return jsonify(parsed)
    
    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Calculation timeout'}), 500
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def parse_output(output):
    """Parse the calculator output to extract results."""
    lines = output.strip().split('\n')
    result = {
        'postfix': '',
        'originalTree': '',
        'simplifiedTree': '',
        'result': ''
    }
    
    for line in lines:
        if 'Postfix expression:' in line:
            result['postfix'] = line.split(':', 1)[1].strip()
        elif 'Original tree (inorder):' in line:
            result['originalTree'] = line.split(':', 1)[1].strip()
        elif 'Simplified tree (inorder):' in line:
            result['simplifiedTree'] = line.split(':', 1)[1].strip()
        elif 'Result:' in line:
            result['result'] = line.split(':', 1)[1].strip()
    
    return result

if __name__ == '__main__':
    # Check if calculator exists
    if not os.path.exists(CALCULATOR_PATH):
        print(f"Error: Calculator not found at {CALCULATOR_PATH}")
        print("Please compile it first: gcc calculator_single_file.c -o calculator_single")
        exit(1)
    
    print("Starting calculator server on http://localhost:5001")
    print("Make sure to open interface.html in your browser")
    app.run(debug=True, port=5001)
