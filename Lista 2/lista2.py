import numpy as np

ROWS = 3
COLS = 4
FINAL = (0, 3)
NEGATIVE = (1, 1)
NEW = (2, 3)
START = (2, 0)
DETERMINISTIC = False

PUTNEWSTATE = False

class State:
    def __init__(self, state=START):
        self.state = state
        self.isEnd = False
        self.determine = DETERMINISTIC

    def giveReward(self):
        if self.state == FINAL:
            return 100
        elif self.state == NEGATIVE:
            return -100
        elif (PUTNEWSTATE == True) and (self.state == NEW):
            return 10
        else:
            return -1

    def isEndFunc(self):
        if (self.state == FINAL):
            self.isEnd = True

    def _chooseActionProb(self, action):
        if action == "up":
            return np.random.choice(["up", "left", "right"], p=[0.8, 0.1, 0.1])
        if action == "down":
            return np.random.choice(["down", "left", "right"], p=[0.8, 0.1, 0.1])
        if action == "left":
            return np.random.choice(["left", "up", "down"], p=[0.8, 0.1, 0.1])
        if action == "right":
            return np.random.choice(["right", "up", "down"], p=[0.8, 0.1, 0.1])

    def nxtPosition(self, action):
        if self.determine:
            if action == "up":
                nxtState = (self.state[0] - 1, self.state[1])
            elif action == "down":
                nxtState = (self.state[0] + 1, self.state[1])
            elif action == "left":
                nxtState = (self.state[0], self.state[1] - 1)
            else:
                nxtState = (self.state[0], self.state[1] + 1)
            #self.determine = False
        else:
            # não determinístico
            action = self._chooseActionProb(action)
            self.determine = True
            nxtState = self.nxtPosition(action)

        # se for possível realizar a ação (sem barreiras)
        if (nxtState[0] >= 0) and (nxtState[0] <= 2):
            if (nxtState[1] >= 0) and (nxtState[1] <= 3):
                if nxtState != (1, 1):
                    return nxtState
        # caso contrário, permanece no mesmo local
        return self.state


class Agent:

    def __init__(self):
        self.states = []  # armazena a posição e ação tomadas naquela posição
        self.actions = ["up", "down", "left", "right"]
        self.State = State()
        self.isEnd = self.State.isEnd
        self.lr = 0.1
        # random choises prob
        self.exp_rate = 0.1
        self.decay_gamma = 1

        # valores iniciais de Q
        self.Q_values = {}
        for i in range(ROWS):
            for j in range(COLS):
                self.Q_values[(i, j)] = {}
                for a in self.actions:
                    self.Q_values[(i, j)][a] = 0  

    def chooseAction(self):
        # escolhe ação com o maior valor esperado
        mx_nxt_reward = 0
        action = ""

        if np.random.uniform(0, 1) <= self.exp_rate:
            action = np.random.choice(self.actions)
        else:
            # ação com maior recompensa
            for a in self.actions:
                current_position = self.State.state
                nxt_reward = self.Q_values[current_position][a]
                if nxt_reward >= mx_nxt_reward:
                    action = a
                    mx_nxt_reward = nxt_reward
    
        return action

    def takeAction(self, action):
        position = self.State.nxtPosition(action)
        # atualiza estado
        return State(state=position)

    def reset(self):
        self.states = []
        self.State = State()
        self.isEnd = self.State.isEnd

    def play(self, rounds=10):
        i = 0
        while i < rounds:
            if self.State.isEnd:
                reward = self.State.giveReward()
                for a in self.actions:
                    self.Q_values[self.State.state][a] = reward
                for s in reversed(self.states):
                    current_q_value = self.Q_values[s[0]][s[1]]
                    reward = current_q_value + self.lr * (self.decay_gamma * reward - current_q_value)
                    self.Q_values[s[0]][s[1]] = round(reward, 3)
                self.reset()
                i += 1
            else:
                action = self.chooseAction()
                self.states.append([(self.State.state), action])
                self.State = self.takeAction(action)
                # marca como fim, no estado final
                self.State.isEndFunc()
                self.isEnd = self.State.isEnd
                


if __name__ == "__main__":
    ag = Agent()
    ag.play(6)
    print("Valores finais de Q \n")
    for i in range(ROWS):
        for j in range(COLS):
            print (i,j)
            print (ag.Q_values[(i,j)])
        print ("\n")
