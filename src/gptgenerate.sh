#!/bin/bash

OUTPUT="gptgather.txt"
CLEAN_COMMENTS=false
PATTERNS=()

# Parse arguments
for arg in "$@"; do
    if [ "$arg" == "-c" ]; then
        CLEAN_COMMENTS=true
    else
        PATTERNS+=("$arg")
    fi
done

# Clear previous output
> "$OUTPUT"

# Gather files
if [[ "${PATTERNS[0]}" == "*" ]]; then
    echo "🌍 Gathering ALL .cpp and .h files..."
    FILES=$(find . -type f \( -name "*.cpp" -o -name "*.h" \))
else
    FILES=""
    for pattern in "${PATTERNS[@]}"; do
        FILES+="$(find . -type f \( -name "*$pattern*.cpp" -o -name "*$pattern*.h" \))"$'\n'
    done
fi

# Process files
for file in $FILES; do
    echo "📄 Adding $file"
    echo "==== FILE: $file ====" >> "$OUTPUT"

    if [ "$CLEAN_COMMENTS" = true ]; then
        # Remove // comments, then collapse multiple empty lines to one
        sed -E 's/\/\/.*//' "$file" | awk 'NF || !blank++' >> "$OUTPUT"
    else
        cat "$file" >> "$OUTPUT"
    fi

    echo -e "\n\n" >> "$OUTPUT"
done

echo "✅ Done. Output saved to $OUTPUT"
