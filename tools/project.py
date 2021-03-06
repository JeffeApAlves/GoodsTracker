#!/usr/bin/env python

"""

@file    project.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1

Carrega configurações do projeto contido no arquivo

"""

import os
import json
import os.path
import locale
import sys
from distutils import *

locale.setlocale(locale.LC_ALL, '')

class PROJECT(object):

    PROJECT_CONF = os.getenv('PROJECT_CONF','./project.conf')

    with open(PROJECT_CONF) as json_data_file:
        configuration = json.load(json_data_file)
        
    # nome do projeto
    NAME = configuration['project']['name']

    # Estrutura de diretorios da solucao

    # diretrio raiz da solução
    HOMEDIR = "%s/%s" % (configuration['project']['workdir'],NAME)
    # ferramnetas
    TOOLDIR = "/".join([HOMEDIR , 'tools'])
    # diretorio do site
    WEBDIR = "/".join([HOMEDIR , '/web'] )
    # embarcado
    PLATFORMDIR = "/".join([HOMEDIR , 'platform'])
    # Diretorio da simulação
    VANETDIR = "/".join([HOMEDIR , 'vanet']) 
    # scripts de inicialização
    STARTUPDIR =  "/".join([HOMEDIR , 'startup'])
    # Arquivos
    REQUERIMENTS_FILE = "/".join([WEBDIR , "requirements.txt"])
    # configurações do supervisor
    SUPERVISORDIR =  "/".join([HOMEDIR , 'supervisor'])

    # adiciona diretorio de ferramentas no path
    sys.path.append(os.path.join(HOMEDIR, 'tools'))


class WEBSERVER(object):

    # IP do webserver da solução
    HOST = PROJECT.configuration['webserver']['host']
    # porta do webserver da solução
    PORT = PROJECT.configuration['webserver']['port']
    # diretrio raiz da solução
    HOMEDIR = PROJECT.configuration['webserver']['home']
    # diretorio do site
    WEBDIR = "/".join([HOMEDIR ,"web"]) 
    # diretorio do arquivos de codnfiguração do supervisor
    SUPERVISORDIR= "/etc/supervisor/conf.d"
    # diretorio startup das aplicações
    STARTUPDIR = "/".join([HOMEDIR,"startup"])