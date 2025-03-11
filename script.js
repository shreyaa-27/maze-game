const WIDTH = 20, HEIGHT = 20;
let maze = [], playerX, playerY, exitX, exitY, enemyX, enemyY, moves = 0;

function generateMaze(difficulty) {
    maze = Array.from({ length: HEIGHT }, () => 
        Array.from({ length: WIDTH }, () => Math.random() * 100 < difficulty ? '#' : '.')
    );

    function placeRandom(char) {
        let x, y;
        do {
            x = Math.floor(Math.random() * HEIGHT);
            y = Math.floor(Math.random() * WIDTH);
        } while (maze[x][y] !== '.');
        maze[x][y] = char;
        return { x, y };
    }

    ({ x: playerX, y: playerY } = placeRandom('P'));
    ({ x: exitX, y: exitY } = placeRandom('E'));
    ({ x: enemyX, y: enemyY } = placeRandom('X'));
}

function renderMaze() {
    let mazeHTML = "";
    for (let i = 0; i < HEIGHT; i++) {
        for (let j = 0; j < WIDTH; j++) {
            let char = maze[i][j];
            let className = char === '#' ? "wall" :
                            char === 'P' ? "player" :
                            char === 'E' ? "exit" :
                            char === 'X' ? "enemy" :
                            "empty";
            mazeHTML += `<div class="cell ${className}">${char}</div>`;
        }
    }
    document.getElementById("maze").innerHTML = mazeHTML;
}

function startGame(difficulty) {
    document.getElementById("difficulty").style.display = "none";
    document.getElementById("game-container").style.display = "block";
    document.getElementById("status").innerText = "";

    generateMaze(difficulty);
    renderMaze();
}

function movePlayer(dx, dy) {
    let newX = playerX + dx, newY = playerY + dy;

    if (newX >= 0 && newX < HEIGHT && newY >= 0 && newY < WIDTH && maze[newX][newY] !== '#') {
        maze[playerX][playerY] = '.';
        playerX = newX; playerY = newY;

        if (playerX === exitX && playerY === exitY) {
            endGame(`🏆 YOU WIN! Moves: ${++moves}`);
            return;
        }

        if (playerX === enemyX && playerY === enemyY) {
            endGame("💀 GAME OVER! The enemy caught you!");
            return;
        }

        maze[playerX][playerY] = 'P';
        moves++;
        setTimeout(moveEnemy, 300);  // Enemy moves **after** player
        renderMaze();
    }
}

function moveEnemy() {
    let bestX = enemyX, bestY = enemyY;
    let bestDist = Math.abs(playerX - enemyX) + Math.abs(playerY - enemyY);

    [[-1, 0], [1, 0], [0, -1], [0, 1]].forEach(([dx, dy]) => {
        let nx = enemyX + dx, ny = enemyY + dy;
        if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH && maze[nx][ny] !== '#' && maze[nx][ny] !== 'E') {
            let dist = Math.abs(playerX - nx) + Math.abs(playerY - ny);
            if (dist < bestDist) { bestX = nx; bestY = ny; bestDist = dist; }
        }
    });

    if (maze[bestX][bestY] !== 'P') {
        maze[enemyX][enemyY] = '.';
        enemyX = bestX; enemyY = bestY;
        maze[enemyX][enemyY] = 'X';
    }

    if (enemyX === playerX && enemyY === playerY) {
        endGame("💀 GAME OVER! The enemy caught you!");
    }

    renderMaze();
}

function endGame(message) {
    document.getElementById("status").innerText = message;
    setTimeout(() => {
        document.getElementById("difficulty").style.display = "block";
        document.getElementById("game-container").style.display = "none";
    }, 3000);  // Wait 3 seconds before returning to difficulty selection
}

document.addEventListener("keydown", (e) => {
    const moves = { "ArrowUp": [-1, 0], "ArrowDown": [1, 0], "ArrowLeft": [0, -1], "ArrowRight": [0, 1],
                    "w": [-1, 0], "s": [1, 0], "a": [0, -1], "d": [0, 1] };
    if (moves[e.key]) movePlayer(...moves[e.key]);
});
