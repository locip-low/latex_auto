% ga_signal_timing.m - 遗传算法求解交叉口信号配时优化
clear; clc;

% 参数设置
pop_size = 100;        % 种群规模
max_gen = 500;         % 最大迭代代数
pc = 0.8;              % 交叉概率
pm = 0.1;              % 变异概率

% 变量上下界 [g1, g2, g3, g4, C]
lb = [15, 15, 15, 15, 60];
ub = [60, 60, 60, 60, 180];

% 初始化种群
pop = repmat(lb, pop_size, 1) + ...
      repmat(ub - lb, pop_size, 1) .* rand(pop_size, 5);

for gen = 1:max_gen
    % 计算适应度
    fitness = zeros(pop_size, 1);
    for i = 1:pop_size
        fitness(i) = 1 / total_delay(pop(i, :));
    end
    
    % 选择（轮盘赌）
    prob = fitness / sum(fitness);
    cum_prob = cumsum(prob);
    new_pop = zeros(pop_size, 5);
    for i = 1:pop_size
        r = rand();
        idx = find(cum_prob >= r, 1);
        new_pop(i, :) = pop(idx, :);
    end
    
    % 交叉（SBX）
    for i = 1:2:pop_size
        if rand() < pc
            [new_pop(i,:), new_pop(i+1,:)] = sbx_crossover(...
                new_pop(i,:), new_pop(i+1,:), lb, ub);
        end
    end
    
    % 变异（多项式变异）
    for i = 1:pop_size
        if rand() < pm
            new_pop(i, :) = poly_mutation(new_pop(i, :), lb, ub);
        end
    end
    
    pop = new_pop;
end

% 输出最优解
[~, best_idx] = max(fitness);
best_solution = pop(best_idx, :);
fprintf('最优配时方案:\n');
fprintf('g1=%.1f g2=%.1f g3=%.1f g4=%.1f C=%.1f\n', best_solution);
fprintf('最小延误: %.1f s/h\n', 1 / fitness(best_idx));