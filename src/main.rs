use std::io;
use termion::raw::IntoRawMode;
use tui::Terminal;
use tui::backend::TermionBackend;
use tui::widgets::{Block, Borders};
use tui::layout::{Layout, Constraint, Direction};

fn main() -> Result<(), io::Error> {
    let stdout = io::stdout().into_raw_mode()?;
    let backend = TermionBackend::new(stdout);
    let mut terminal = Terminal::new(backend)?;

    terminal.draw(|f| {
        let chunks = Layout::default()
            .direction(Direction::Vertical)
            .margin(1)
            .constraints([
                Constraint::Percentage(20),
                Constraint::Percentage(80)
            ].as_ref())
            .split(f.size());

        let block = Block::default()
            .title("elemao")
            .borders(Borders::ALL);

        let block1 = Block::default()
            .title("facts")
            .borders(Borders::ALL);

        f.render_widget(block, chunks[0]);
        f.render_widget(block1, chunks[1]);
    });

    Ok(())
}
