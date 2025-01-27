from typing import Optional, Dict, List
from .storage import Storage

class PasswordManager:
    def __init__(self, storage: Storage):
        self.storage = storage

    def create_entry(self, service: str, username: str, password: str) -> bool:
        """Create a new password entry"""
        entry = {
            'service': service,
            'username': username,
            'password': password
        }
        return self.storage.write(service, entry)

    def get_entry(self, service: str) -> Optional[Dict]:
        """Retrieve an entry by service name"""
        return self.storage.read(service)

    def update_entry(self, service: str, username: str, password: str) -> bool:
        """Update an existing entry"""
        entry = {
            'service': service,
            'username': username,
            'password': password
        }
        return self.storage.update(service, entry)

    def delete_entry(self, service: str) -> bool:
        """Delete an entry"""
        return self.storage.delete(service)

    def list_entries(self) -> List[str]:
        """List all stored services"""
        return self.storage.list_services()
