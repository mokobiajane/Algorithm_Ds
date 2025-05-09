import subprocess
import base64
import random
import string

ASCII85_EXE = './ascii85'

def run_cmd(args):
    """Run the ascii85 CLI with arguments and return code, stdout, stderr."""
    try:
        result = subprocess.run(
            [ASCII85_EXE] + args,
            capture_output=True,
            text=True,
            timeout=5
        )
        return result.returncode, result.stdout.strip(), result.stderr.strip()
    except Exception as e:
        return -1, '', str(e)

def generate_printable_ascii(length):
    # Generate a sttring c.p ASCII characters (safe for CLI)
    return ''.join(random.choices(string.printable[:95], k=length))

def test_valid_data():
    print("[+] Valid data test:")
    for _ in range(5):
        text_input = generate_printable_ascii(random.randint(10, 50))
        try:
            py_encoded = base64.a85encode(text_input.encode()).decode()
        except Exception as e:
            print(f"    [SKIP] Couldn't encode with Python: {e}")
            continue

        # Test decoder
        code, cpp_decoded, err = run_cmd(['-d', f"<~{py_encoded}~>"])
        if code != 0:
            print(f"    [FAIL] C++ decoder exited with {code}, error: {err}")
        elif cpp_decoded != text_input:
            print("    [FAIL] Mismatch between Python input and C++ decoded output.")
        else:
            print("    [OK] Decode match.")

        # Test encoder
        code, cpp_encoded, err = run_cmd(['-e', text_input])
        if code != 0:
            print(f"    [FAIL] C++ encoder exited with {code}, error: {err}")
            continue
        try:
            py_decoded = base64.a85decode(cpp_encoded[2:-2])  # Strip <~ ~>
        except Exception as e:
            print(f"    [FAIL] Python decode failed on C++ output: {e}")
            continue
        if py_decoded.decode(errors='replace') != text_input:
            print("    [FAIL] Python-decoded output doesn't match input.")
        else:
            print("    [OK] Encode match.")

def test_invalid_data():
    print("\n[+] Invalid data test:")
    invalid_inputs = [
        "<~bad@input~>",
        "<~zzzzz~>",
        "not even wrapped",
        "<~BOu!rDZ",      # missing ~>
        "BOu!rDZ~>",      # missing <~
        "<~BOu!rDZ~>extra",  # trailing junk
        "<~AB@!#~>",   # garbage
    ]
    for invalid in invalid_inputs:
        code, _, err = run_cmd(['-d', invalid])
        if code == 0:
            print(f"    [FAIL] Invalid input '{invalid}' returned code 0")
        else:
            print(f"    [OK] Invalid input correctly failed with code {code}")

if __name__ == "__main__":
    test_valid_data()
    test_invalid_data()