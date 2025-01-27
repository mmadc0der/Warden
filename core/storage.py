import os
import json
from typing import Optional, Dict, List

class Storage:
    BLOCK_SIZE = 4096  # Fixed block size in bytes
    
    def __init__(self, data_dir: str):
        self.data_dir = data_dir
        os.makedirs(data_dir, exist_ok=True)
        self.index_file = os.path.join(data_dir, 'index.json')
        self._load_index()

    def _load_index(self):
        """Load or create the index file"""
        if os.path.exists(self.index_file):
            with open(self.index_file, 'r') as f:
                self.index = json.load(f)
        else:
            self.index = {}
            self._save_index()

    def _save_index(self):
        """Save the index file"""
        with open(self.index_file, 'w') as f:
            json.dump(self.index, f)

    def _get_block_path(self, block_id: str) -> str:
        """Get the path to a block file"""
        return os.path.join(self.data_dir, f'block_{block_id}.dat')

    def write(self, key: str, data: Dict) -> bool:
        """Write data to a new block"""
        if key in self.index:
            return False

        # Convert data to bytes and pad to block size
        data_bytes = json.dumps(data).encode()
        padded_data = data_bytes.ljust(self.BLOCK_SIZE, b'\0')

        # Generate block ID and save data
        block_id = str(len(self.index))
        block_path = self._get_block_path(block_id)

        with open(block_path, 'wb') as f:
            f.write(padded_data)

        self.index[key] = block_id
        self._save_index()
        return True

    def read(self, key: str) -> Optional[Dict]:
        """Read data from a block"""
        if key not in self.index:
            return None

        block_id = self.index[key]
        block_path = self._get_block_path(block_id)

        with open(block_path, 'rb') as f:
            data_bytes = f.read(self.BLOCK_SIZE)

        # Remove padding and parse JSON
        data_str = data_bytes.rstrip(b'\0').decode()
        return json.loads(data_str)

    def update(self, key: str, data: Dict) -> bool:
        """Update data in an existing block"""
        if key not in self.index:
            return False

        block_id = self.index[key]
        block_path = self._get_block_path(block_id)

        data_bytes = json.dumps(data).encode()
        padded_data = data_bytes.ljust(self.BLOCK_SIZE, b'\0')

        with open(block_path, 'wb') as f:
            f.write(padded_data)

        return True

    def delete(self, key: str) -> bool:
        """Delete a block"""
        if key not in self.index:
            return False

        block_id = self.index[key]
        block_path = self._get_block_path(block_id)

        if os.path.exists(block_path):
            os.remove(block_path)

        del self.index[key]
        self._save_index()
        return True

    def list_services(self) -> List[str]:
        """List all services in the index"""
        return list(self.index.keys())
