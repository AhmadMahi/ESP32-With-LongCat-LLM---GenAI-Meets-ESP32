# Make sure you install requests if not already
!pip install requests

import requests

# === Configuration ===
API_KEY = ""  # replace with your actual LongCat API key
ENDPOINT = "https://api.longcat.chat/openai/v1/chat/completions"

# === Request payload ===
payload = {
    "model": "LongCat-Flash-Chat",
    "messages": [
        {"role": "user", "content": "Hello, can you introduce yourself, in very detail"}
    ],
    "max_tokens": 10000,
    "temperature": 0.7
}

headers = {
    "Authorization": f"Bearer {API_KEY}",
    "Content-Type": "application/json"
}

# === Send request ===
response = requests.post(ENDPOINT, headers=headers, json=payload)

# === Process response ===
if response.status_code == 200:
    resp_json = response.json()
    # Extracting the assistant's message content
    assistant_message = resp_json["choices"][0]["message"]["content"]
    print("Assistant says:", assistant_message)
else:
    print(f"Request failed with status {response.status_code}")
    print("Response body:", response.text)
