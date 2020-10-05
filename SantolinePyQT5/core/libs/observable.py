class Observable:
    def __init__(self):
        self.observers_ = []
        
    def addObserver(self, observer):
        self.observers_.append(observer)
        
    def removeObserver(self, observer):
        self.observers_.remove(observer)

    def notifyObservers(self):
        for i in self.observers_:
            i.update(self)

class Observer:
    def __init__(self):
        # To implement in subclass
        pass
        
    def update(self, observable):
        # To implement in subclass
        print("update")
        pass
