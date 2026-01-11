#!/bin/bash
# workflow.sh - Complete build, sign, and package workflow

if [ $# -ne 1 ]; then
    echo "Usage: $0 <source.c>"
    echo "Example: $0 GAMER.c"
    exit 1
fi

SOURCE_FILE="$1"

# Check if source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: File not found: $SOURCE_FILE"
    exit 1
fi

# Extract base name (remove .c extension)
BASENAME="${SOURCE_FILE%.c}"

echo "=== Secure Update Workflow ==="
echo "Source: $SOURCE_FILE"
echo ""

# Step 1: Generate keys if they don't exist
if [ ! -f "REDACTED_PRIVATE_KEY.pem" ]; then
    echo "[1/5] Generating RSA keys..."
    openssl genrsa -out REDACTED_PRIVATE_KEY.pem 4096 2>/dev/null
    openssl rsa -in REDACTED_PRIVATE_KEY.pem -pubout -out REDACTED_PUBLIC_KEY.pem 2>/dev/null
    chmod 600 REDACTED_PRIVATE_KEY.pem
    chmod 644 REDACTED_PUBLIC_KEY.pem
    echo "✓ Keys generated (REDACTED_PRIVATE_KEY.pem, REDACTED_PUBLIC_KEY.pem)"
else
    echo "[1/5] Using existing keys"
fi

# Step 2: Compile the C program
echo "[2/5] Compiling $SOURCE_FILE..."
gcc -o "$BASENAME" "$SOURCE_FILE"
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi
echo "✓ Compiled: $BASENAME"

# Step 3: Sign the binary
echo "[3/5] Signing binary..."
openssl dgst -sha256 -sign REDACTED_PRIVATE_KEY.pem -out "${BASENAME}.sig" "${BASENAME}"
if [ $? -ne 0 ]; then
    echo "❌ Signing failed!"
    exit 1
fi
echo "✓ Signature created: ${BASENAME}.sig"

# Step 4: Package as .raining
echo "[4/5] Creating .raining package..."
tar -czf "${BASENAME}.raining" "${BASENAME}" "${BASENAME}.sig"
if [ $? -ne 0 ]; then
    echo "❌ Packaging failed!"
    exit 1
fi
echo "✓ Package created: ${BASENAME}.raining"

# Step 5: Cleanup temporary files
echo "[5/5] Cleaning up..."
rm "${BASENAME}" "${BASENAME}.sig"
echo "✓ Cleanup complete"

echo ""
echo "=========================================="
echo "SUCCESS! Created: ${BASENAME}.raining"
echo "=========================================="
echo ""
echo "Distribute this file to users."
echo "Public key for users: REDACTED_PUBLIC_KEY.pem"
