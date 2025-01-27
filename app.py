import os
import cmd
from cli.commands import CommandHandler
from core.manager import PasswordManager
from core.storage import Storage
from utils.generator import generate_password
from utils.validation import validate_service_name, validate_username, validate_password

class WardenCLI(cmd.Cmd):
    intro = 'Welcome to Warden Password Manager. Type help or ? to list commands.\n'
    prompt = 'warden> '

    def __init__(self):
        super().__init__()
        data_dir = os.path.join(os.path.dirname(__file__), 'data')
        storage = Storage(data_dir)
        manager = PasswordManager(storage)
        self.handler = CommandHandler(manager)

    def do_add(self, arg):
        """Add a new password entry: add <service> <username> <password>"""
        args = arg.split()
        if len(args) != 3:
            print("Usage: add <service> <username> <password>")
            return

        service, username, password = args
        if not all([validate_service_name(service), 
                   validate_username(username), 
                   validate_password(password)]):
            print("Invalid input format")
            return

        if self.handler.add_password(service, username, password):
            print(f"Password added for service: {service}")
        else:
            print(f"Service {service} already exists")

    def do_get(self, arg):
        """Retrieve a password: get <service>"""
        if not arg:
            print("Usage: get <service>")
            return

        if not validate_service_name(arg):
            print("Invalid service name")
            return

        entry = self.handler.get_password(arg)
        if entry:
            print(f"Service: {entry['service']}")
            print(f"Username: {entry['username']}")
            print(f"Password: {entry['password']}")
        else:
            print(f"No entry found for service: {arg}")

    def do_list(self, arg):
        """List all stored services"""
        services = self.handler.list_services()
        if services:
            print("\nStored services:")
            for service in services:
                print(f"- {service}")
        else:
            print("No stored services found")

    def do_delete(self, arg):
        """Delete a password entry: delete <service>"""
        if not arg:
            print("Usage: delete <service>")
            return

        if not validate_service_name(arg):
            print("Invalid service name")
            return

        if self.handler.delete_password(arg):
            print(f"Deleted password for service: {arg}")
        else:
            print(f"No entry found for service: {arg}")

    def do_generate(self, arg):
        """Generate a secure password: generate [length]"""
        try:
            length = int(arg) if arg else 16
            if length < 8 or length > 128:
                print("Password length must be between 8 and 128 characters")
                return
            password = generate_password(length)
            print(f"Generated password: {password}")
        except ValueError:
            print("Invalid length specified")

    def do_exit(self, arg):
        """Exit the password manager"""
        print("Goodbye!")
        return True

if __name__ == '__main__':
    WardenCLI().cmdloop()