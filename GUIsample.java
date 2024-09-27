import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class GUIsample {
    public static void main(String[] args) {
        // Create the frame
        JFrame F = new JFrame("Sample GUI");
        F.setSize(400, 300);
        F.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Set layout
        F.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL; // Fill space horizontally
        gbc.insets = new Insets(10, 10, 10, 10);  // Add padding between components

        // Create components
        JLabel L1 = new JLabel("Input:");
        JLabel L2 = new JLabel("Duplicate:");
        JTextArea T1 = new JTextArea(1, 20);
        JTextArea T2 = new JTextArea(1, 20);
        JButton B = new JButton("Do");

        // Set non-editable property for T2 (Optional: if you want T2 to be read-only)
        T2.setEditable(false);

        // Position for L1 (Input:)
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.LINE_END; // Align to the right side
        F.add(L1, gbc);

        // Position for T1 (Input TextArea)
        gbc.gridx = 1;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.LINE_START; // Align to the left side
        F.add(T1, gbc);

        // Position for L2 (Duplicate:)
        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.LINE_END; // Align to the right side
        F.add(L2, gbc);

        // Position for T2 (Duplicate TextArea)
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.anchor = GridBagConstraints.LINE_START; // Align to the left side
        F.add(T2, gbc);

        // Position for B (Button)
        gbc.gridx = 0;
        gbc.gridy = 2;
        gbc.gridwidth = 2; // Span across 2 columns
        gbc.anchor = GridBagConstraints.CENTER; // Center the button
        F.add(B, gbc);

        // Action listener for the button
        B.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Get text from T1 and set it in T2
                String s1 = T1.getText();
                T2.setText(s1);
            }
        });

        // Show the frame
        F.setVisible(true);
    }
}
