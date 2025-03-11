// Daragh Kearney / SmartHome JS 
function showSection(sectionId) {
    // Hide all sections
    document.querySelectorAll('.container').forEach(section => {
        section.style.display = 'none';
    });

    // Show the selected section
    document.getElementById(sectionId).style.display = 'block';
}

// Show home section by default on page load
window.onload = function() {
    showSection('home');
};
