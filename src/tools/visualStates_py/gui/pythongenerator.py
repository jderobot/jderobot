from gui.generator import Generator
from gui.transitiontype import TransitionType
import os

class PythonGenerator(Generator):
    def __init__(self, libraries, configs, interfaceHeaders, states):
        self.libraries = libraries
        self.configs = configs
        self.interfaceHeaders = interfaceHeaders
        self.states = states

    def getAllStates(self):
        addedStates = {}
        allStates = []
        for state in self.states:
            if state.id not in addedStates:
                addedStates[state.id] = state
                allStates.append(state)

            for childState in state.getChildren():
                if childState.id not in addedStates:
                    addedStates[childState.id] = childState
                    allStates.append(childState)

        return allStates

    def getAllTransitions(self):
        addedTransitions = {}
        transitions = []
        for state in self.states:
            for tran in state.getOriginTransitions():
                # print('1tran.id:' + str(tran.id) + ' origin:' + str(tran.origin.id) + ' dest:' + str(tran.destination.id))
                if tran.id not in addedTransitions:
                    addedTransitions[tran.id] = tran
                    transitions.append(tran)
            for childState in state.getChildren():
                for tran in childState.getOriginTransitions():
                    # print('2tran.id:' + str(tran.id) + ' origin:' + str(tran.origin.id) + ' dest:' + str(
                    #     tran.destination.id))
                    if tran.id not in addedTransitions:
                        addedTransitions[tran.id] = tran
                        transitions.append(tran)

        return transitions

    def generate(self, projectPath, projectName):
        stringList = []
        self.generateImports(stringList)
        self.generateStateClasses(stringList)
        self.generateTransitionClasses(stringList)
        self.generateInterfaces(stringList)
        self.generateMain(stringList)
        sourceCode = ''.join(stringList)
        fp = open(projectPath + os.sep + projectName + '.py', 'w')
        fp.write(sourceCode)
        fp.close()

        stringList = []
        self.generateCfg(stringList)
        fp = open(projectPath + os.sep + projectName + '.cfg', 'w')
        fp.write(''.join(stringList))
        fp.close()

        os.system('chmod +x ' + projectPath + os.sep + projectName + '.py')


    def generateImports(self, headerStr):
        mystr = '''#!/usr/bin/python3
# -*- coding: utf-8 -*-
import sys
import easyiceconfig as EasyIce
from gui.codegen.state import State
from gui.codegen.temporaltransition import TemporalTransition
from gui.codegen.conditionaltransition import ConditionalTransition

'''
        headerStr.append(mystr)
        for lib in self.libraries:
            headerStr.append('import ')
            headerStr.append(lib)
            headerStr.append('\n')
        headerStr.append('\n')

        for cfg in self.configs:
            headerStr.append('from jderobot import ')
            headerStr.append(cfg['interface'])
            headerStr.append('Prx\n')

        headerStr.append('\n')

        return headerStr

    def generateStateClasses(self, stateStr):
        for state in self.getAllStates():
            self.generateStateClass(state, stateStr)

    def generateStateClass(self, state, stateStr):
        stateStr.append('class State')
        stateStr.append(str(state.id))
        stateStr.append('(State):\n')

        if len(state.getVariables()) > 0:
            stateStr.append('\tdef __init__(self, id, initial, config, cycleDuration, parent=None):\n')
            stateStr.append('\t\tsuper().__init__(id, initial, config, cycleDuration, parent=None)\n')
            for varLine in state.getVariables().split('\n'):
                stateStr.append('\t\t' + varLine + '\n')
            stateStr.append('\n')

        stateStr.append('\tdef runCode(self):\n')
        if len(state.getCode()) > 0:
            for codeLine in state.getCode().split('\n'):
                stateStr.append('\t\t' + codeLine + '\n')
        else:
            stateStr.append('\t\tpass\n')
        stateStr.append('\n')

        if len(state.getFunctions()) > 0:
            for funcLine in state.getFunctions().split('\n'):
                stateStr.append('\t' + funcLine + '\n')
        stateStr.append('\n')

    def generateInterfaces(self, interfaceStr):
        mystr = '''class Interfaces():
\tdef __init__(self):
\t\tself.ic = None
'''
        interfaceStr.append(mystr)
        for cfg in self.configs:
            interfaceStr.append('\t\tself.' + cfg['name'] + ' = None\n')

        interfaceStr.append('\t\tself.connectProxies()\n\n')

        interfaceStr.append('\tdef connectProxies(self):\n')
        interfaceStr.append('\t\tself.ic = EasyIce.initialize(sys.argv)\n')

        for cfg in self.configs:
            interfaceStr.append('\t\tself.' + cfg['name'] + ' = self.ic.propertyToProxy("automata.' + cfg['name'] + '.Proxy")\n')
            interfaceStr.append('\t\tif not self.' + cfg['name'] + ':\n')
            interfaceStr.append('\t\t\traise Exception("could not create proxy with name:' + cfg['name'] + '")\n')
            interfaceStr.append('\t\tself.' + cfg['name'] + ' = ' + cfg['interface'] + 'Prx.checkedCast(self.' + cfg['name'] + ')\n')
            interfaceStr.append('\t\tif not self.' + cfg['name'] + ':\n')
            interfaceStr.append('\t\t\traise Exception("invalid proxy automata.myMotors.Proxy")\n')

        interfaceStr.append('\n')

        interfaceStr.append('\tdef destroyProxies(self):\n')
        interfaceStr.append('\t\tif self.ic is not None:\n')
        interfaceStr.append('\t\t\tself.ic.destroy()\n\n')

    def generateTransitionClasses(self, tranStr):
        for tran in self.getAllTransitions():
            if tran.getType() == TransitionType.CONDITIONAL:
                tranStr.append('class Tran' + str(tran.id) + '(ConditionalTransition):\n')
                tranStr.append('\tdef checkCondition(self):\n')
                for checkLine in tran.getCondition().split('\t'):
                    tranStr.append('\t\t' + checkLine + '\n')
                tranStr.append('\n')
            elif tran.getType() == TransitionType.TEMPORAL:
                tranStr.append('class Tran' + str(tran.id) + '(TemporalTransition):\n\n')

            tranStr.append('\tdef runCode(self):\n')
            if len(tran.getCode()) > 0:
                for codeLine in tran.getCode().split('\n'):
                    tranStr.append('\t\t' + codeLine + '\n')
                tranStr.append('\n')
            else:
                tranStr.append('\t\tpass\n\n')

    def generateMain(self, mainStr):
        mainStr.append('if __name__ == "__main__":\n')
        mainStr.append('\tinterfaces = Interfaces()\n')
        for state in self.getAllStates():
            mainStr.append('\tstate' + str(state.id) + ' = State' + str(state.id) +
                           '(' + str(state.id) + ', ' + str(state.initial) + ', interfaces, ' +
                           str(state.getTimeStep()))
            if state.parent is None:
                mainStr.append(', None)\n')
            else:
                mainStr.append(', state' + str(state.parent.id) + ')\n')

        mainStr.append('\n')

        # create and add transitions to their origins
        for tran in self.getAllTransitions():
            if tran.getType() == TransitionType.TEMPORAL:
                mainStr.append('\ttran' + str(tran.id) + ' = Tran' + str(tran.id) +
                               '(' + str(tran.id) + ', ' + str(tran.destination.id) + ', ' + str(tran.getTemporalTime()) + ')\n')
            elif tran.getType() == TransitionType.CONDITIONAL:
                mainStr.append('\ttran' + str(tran.id) + ' = Tran' + str(tran.id) +
                               '(' + str(tran.id) + ', ' + str(tran.destination.id) + ')\n')

            mainStr.append('\tstate' + str(tran.origin.id) + '.addTransition(tran' + str(tran.id) + ')\n\n')

        mainStr.append('\ttry:\n')
        # start threads
        for state in self.states:
            mainStr.append('\t\tstate' + str(state.id) + '.startThread()\n')

        # join threads
        for state in self.states:
            mainStr.append('\t\tstate' + str(state.id) + '.join()\n')

        mainStr.append('\t\tinterfaces.destroyProxies()\n')
        mainStr.append('\t\tsys.exit(0)')
        mainStr.append('\texcept:\n')
        for state in self.states:
            mainStr.append('\t\tstate' + str(state.id) + '.stop()\n')
        # join threads
        for state in self.states:
            mainStr.append('\t\tstate' + str(state.id) + '.join()\n')
        mainStr.append('\t\tinterfaces.destroyProxies()\n')
        mainStr.append('\t\tsys.exit(1)\n')

































