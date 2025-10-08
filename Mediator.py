class ChatMediator:
    def send_message(self, message, user):
        pass

class ChatRoom(ChatMediator):
    def __init__(self):
        self._users = []

    def add_user(self, user):
        self._users.append(user)
        user.chat_mediator = self

    def send_message(self, message, user_from):
        for user in self._users:
            if user != user_from:
                user.receive_message(message)

class User:
    def __init__(self, name):
        self.name = name
        self.chat_mediator = None

    def send_message(self, message):
        if self.chat_mediator:
            print(f"[{self.name}] отправляет: {message}")
            self.chat_mediator.send_message(message, self)
        else:
            print(f"[{self.name}] не в чате!")

    def receive_message(self, message):
        print(f"[{self.name}] получил: {message}")

if __name__ == "__main__":
    chat_room = ChatRoom()

    alice = User("Алиса")
    bob = User("Боб")
    charlie = User("Чарли")

    chat_room.add_user(alice)
    chat_room.add_user(bob)
    chat_room.add_user(charlie)
    alice.send_message("Всем привет!")
