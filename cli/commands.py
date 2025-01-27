from typing import Optional
from core.manager import PasswordManager

class CommandHandler:
    def __init__(self, manager: PasswordManager):
        self.manager = manager

    def add_password(self, service: str, username: str, password: str) -> bool:
        """Add a new password entry"""
        return self.manager.create_entry(service, username, password)

    def get_password(self, service: str) -> Optional[dict]:
        """Retrieve password entry by service name"""
        return self.manager.get_entry(service)

    def list_services(self) -> list:
        """List all stored services"""
        return self.manager.list_entries()

    def delete_password(self, service: str) -> bool:
        """Delete a password entry"""
        return self.manager.delete_entry(service)

    def update_password(self, service: str, username: str, password: str) -> bool:
        """Update an existing password entry"""
        return self.manager.update_entry(service, username, password)
