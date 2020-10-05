# -*- coding: utf-8 -*-
#!/usr/bin/env python

"""Tube nommé multiplexé.

Module simple qui permet de faire des tubes nommé multiplexé. La partie à importer et
qui est la plus importante est la classe Pipe.

La classe est thread safe, et stock les messages à envoyer tant que la connexion
avec le tube n'est pas établie.
Tant que la connexion n'est pas établie, une tentative de connexion à lieu toute
les deux secondes.
Lors que la connexion s'effectue, tout les messages sont envoyé d'un coup.
Il n'y a pas de timeout ou de fin, tant qu'on ne demande pas explicitement au
programme de s'arreter, il continue.

Auteur : Antoine Porcheron-Roche - antoine.pr@live.fr
Date : 25 Juillet 2018
"""

import threading
import codecs
import struct
import time
import queue
from abc import ABC, abstractmethod

class DisconnectedPipeException(Exception):
    """Exception pour les interaction avec le tube qui nécessite que celui ci soit
    connecté.
    """
    def __init__(self, message):
        super(DisconnectedPipeException, self).__init__(message)

class PipeState(ABC):
    """Classe abstraite qui représente la classe mère des états d'un tube nommé."""
    
    """Attribut statique pour annoncé un état connecté."""
    CONNECTED = True
    
    """Attribut statique pour annoncé un état déconnecté."""
    DISCONNECTED = False
    
    def __init__(self, stated_pipe):
        """Constructeur.
        Arguments :
            stated_pipe : l'objet qui est géré par ce patron état.
        """
        super(PipeState, self).__init__()
        self._owner = stated_pipe
        self._pipe = self._owner._pipe

    @abstractmethod
    def connect(self):
        """Permet d'effectuer la connexion au tube nommé si celle ci n'est pas
        déjà faite.
        """
        pass

    @abstractmethod
    def write(self):
        """Permet de faire l'écriture sur le tube nommé si cela est possible."""
        pass

    @abstractmethod
    def read(self):
        """Permet d'effectuer la lecture sur le tube nommé si cela est possible."""
        pass

    @abstractmethod
    def close(self):
        """Permet de fermer le tube nommé si cela est possible."""
        pass

    @abstractmethod
    def state(self):
        """Permet d'obtenir l'état actuel du tube nommé."""
        pass

class PipeStateConnected(PipeState):
    """Classe qui représente le tube nommé connecté. Cet état permet donc d'effectuer
    les actions de lecture et d'écriture.
    """
    def __init__(self, stated_pipe):
        """Constructeur. Fait appelle au constructeur de PipeState."""
        super(PipeStateConnected, self).__init__(stated_pipe)

    def connect(self, pipename):
        """Connexion au tube nommé. Vu que dans cet état on la suppose déjà faite,
        il n'y a rien à faire dans cette fonction.

        Arguments :
            pipename : Nom du tube nommé.
        """
        pass

    def write(self, message):
                """Ecriture dans le tube nommé.
                Si pour une raison ou une autre l'écriture ne peut pas avoir lieu,
                le tube nommé passe à l'état déconnecté.

                Arguments :
                        message Le message à écrire sur le pipe.
                """
                try:
                    self._pipe.write(struct.pack('i', len(message)))
                    self._pipe.flush()
                    self._pipe.write(message.encode("ascii"))
                    self._pipe.flush()
                except Exception as e:
                    self._owner._state = PipeStateDisconnected(self._owner)

    def read(self):
                """Effectue une lecture sur le tube nommé. Si pour une raison ou une autre
                la lecture ne peut s'effectuer, le tube nommé passe à l'état déconnecté, et
                cette fonction renvoie "None".

                Retours :
                        string : Renvoie le message lu sur le tube, ou None si la lecture n'a pas pu
                        se faire.
                """
                try:
                    size = self._pipe.read(4)
                    message = self._pipe.read(struct.unpack("<i", size)[0])
                    return message.decode("ascii")
                except Exception as e:
                    self._owner._state = PipeStateDisconnected(self._owner)
                    return self._owner._state.read()

    def close(self):
        """Ferme le pipe, et passe à l'état déconnecté."""
        self._pipe.close()
        self._owner._state = PipeStateDisconnected(self._owner)

    def state(self):
        """Renvoie connecté, via l'attribut global CONNECTED."""
        return PipeState.CONNECTED

class PipeStateDisconnected(PipeState):
    """Représente le tube nommé à l'état déconnecté."""

    def __init__(self, stated_pipe):
        """Constructeur. Fait appelle au constructeur de PipeState."""
        super(PipeStateDisconnected, self).__init__(stated_pipe)

    def connect(self, pipename):
        """Permet de se connecté au tube nommé.

        Arguments :
            pipename : Nom du tube nommé.
        """
        self._owner._pipe = open(r'\\.\pipe\{}'.format(pipename), 'w+b', 0)
        self._owner._state = PipeStateConnected(self._owner)

    def write(self, message):
        """Renvoie une erreur DisconnectedPipeException,
        on ne peut pas écrire sur le tube nommé si la connexion n'est pas faite.

        Arguments :
            message : Message à écrire sur le tube nommé.
        """
        raise DisconnectedPipeException("Tube deconnecté.")

    def read(self):
        """Renvoie une erreur DisconnectedPipeException,
        on ne peut pas lire sur le tube nommé si la connexion n'est pas faite.
        """
        raise DisconnectedPipeException("Tube deconnecté.")

    def close(self):
        """Ne fait rien, il n'y a rien à faire pour la fermeture du tube lorsque
        l'on est déjà déconnecté.
        """
        pass

    def state(self):
        """Renvoie déconnecté, via l'attribut global DISCONNECTED."""
        return PipeState.DISCONNECTED


class PipeMultiplexed(object):
    """Tube multiplexé, peut donc effectué la communication dans les deux sens.
    Ceci n'est qu'un client, si le tube n'est pas déjà existant, cette classe ne
    peut pas le créer.
    """

    def __init__(self):
        """Constructeur par défaut.
        Par défaut, le tube est dans l'état déconnecté
        """
        self._pipe = None
        self._state = PipeStateDisconnected(self)

    def connect(self, pipename):
        """Permet d'effectuer la connexion au tube qui à pour nom "pipename"

        Arguments :
            pipename : Le nom du tube nommé.
        """
        self._state.connect(pipename)

    def write(self, message):
        """Permet de faire l'écriture sur le tube nommé.

        Arguments :
            message : Le message à écrire sur le tube nommé.
        """
        self._state.write(message)

    def read(self):
        """Permet de faire la lecture sur le tube nommé. Opération bloquante.
        Peut retourné None en cas d'erreur.

        Retours :
            string : Ce qui a été lu sur le tube nommé, ou None en cas d'erreur.
        """
        return self._state.read()

    def close(self):
        """Permet de faire la fermeture du tube nommé."""
        self._state.close()

    def state(self):
        """Permet d'obtenir l'état courant du tube nommé.

        Retours :
            bool : PipeState.CONNECTED ou PipeState.DISCONNECTED.
        """
        return self._state.state()
        
class Pipe(threading.Thread):
    """
    Classe façade qui simplifie l'usage des tube nommés, et les rends threadsafe.
    Se base sur l'implémentation des thread stoppable décrite ici : 
    https://www.oreilly.com/library/view/python-cookbook-2nd/0596007973/ch09s03.html
    """

    def __init__(self, pipename):
        """Constructeur d'un tube nommé.

        Arguments :
            pipename : Le nom du tube nommé.
        """
        super(Pipe, self).__init__()
        self._pipe = PipeMultiplexed()
        self._pipename = pipename
        self._writings = queue.Queue()
        self._stopEvent = threading.Event()

    def state_checker(self):
        """Fonction qui effectue la connexion avec le pipe si celle ci n'est pas
        active.
        Tant que la connexion n'arrive pas à s'effectuer, cette fonction cherche à
        se connecter toute les deux secondes.
        S'arrête uniquement lorsque l'on ferme le tube.
        """
        print(self._pipe.state())
        while (self._pipe.state() == PipeState.DISCONNECTED):
            try:
                self._pipe.connect(self._pipename)
            except Exception as e:
                time.sleep(1)
        print("finish")

    def write(self, message):
        """Permet d'ajouter le message à écrire dans la file.
        Tout les messages de la files sont écrits lorsque la connexion s'effectue.
        Arguments :
            message - Le message à ajouter à la file des messages.
        """
        self._writings.put(message)

    def read(self):
        """Permet de faire la lecture sur le tube nommé. Opération bloquante."""
        self.state_checker()
        result = self._pipe.read()
        return result

    def run(self):
        try:
            while not self._stopEvent.isSet():
                self.state_checker()
                self._pipe.write(self._writings.get())
        except DisconnectedPipeException as e:
            pass
            
    def join(self, timeout=None):
        self._stopEvent.set()
        self._pipe.close()
        super().join(timeout)