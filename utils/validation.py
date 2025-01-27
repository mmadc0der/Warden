def validate_service_name(service: str) -> bool:
    """Validate service name format"""
    return bool(service and service.strip() and len(service) <= 255)

def validate_username(username: str) -> bool:
    """Validate username format"""
    return bool(username and username.strip() and len(username) <= 255)

def validate_password(password: str) -> bool:
    """Validate password format"""
    return bool(password and len(password) <= 4096)
