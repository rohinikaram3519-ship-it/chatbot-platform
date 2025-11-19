async function fetchStats() {
    try {
        const response = await fetch('http://localhost:8080/stats');
        const data = await response.json();
        
        document.getElementById('total-messages').textContent = data.total_messages || 0;
        
        const categoriesDiv = document.getElementById('categories');
        categoriesDiv.innerHTML = '';
        
        if (data.categories && data.categories.length > 0) {
            data.categories.forEach(cat => {
                const div = document.createElement('div');
                div.className = 'category-item';
                div.innerHTML = `<span>${cat.name}</span><span>${cat.count} messages</span>`;
                categoriesDiv.appendChild(div);
            });
        }
    } catch (error) {
        console.error('Error fetching stats:', error);
    }
}

async function fetchLogs() {
    try {
        const response = await fetch('http://localhost:8080/logs');
        const data = await response.json();
        
        const logsDiv = document.getElementById('logs');
        logsDiv.innerHTML = '';
        
        if (data.logs && data.logs.length > 0) {
            data.logs.forEach(log => {
                const div = document.createElement('div');
                div.className = 'log-item';
                div.innerHTML = `
                    <p><strong>User:</strong> ${log.user_id}</p>
                    <p><strong>Message:</strong> ${log.message}</p>
                    <p><strong>Response:</strong> ${log.response}</p>
                    <p><strong>Category:</strong> ${log.category}</p>
                    <p><small>${log.timestamp}</small></p>
                `;
                logsDiv.appendChild(div);
            });
        } else {
            logsDiv.innerHTML = '<p>No messages yet</p>';
        }
    } catch (error) {
        console.error('Error fetching logs:', error);
    }
}

// Refresh data every 5 seconds
fetchStats();
fetchLogs();
setInterval(() => {
    fetchStats();
    fetchLogs();
}, 5000);