module key_filter #(
    parameter I_CLK_FREQ = 50_000_000   // 50MHz, 20ns
) (
    input  logic i_clk,
    input  logic i_rst_n,
    input  logic i_key,
    output logic o_key_filtered
);
    localparam CNT_20MS_MAX = 1_000_000;   // 20ms/20ns

// Sync input signal
    logic r_key_sync0;
    logic r_key_sync1;

    always_ff @( posedge i_clk, negedge i_rst_n ) begin : alff_KeyBuffer
        if (!i_rst_n) begin
            r_key_sync0 <= 1;
            r_key_sync1 <= 1;
        end
        else begin
            r_key_sync0 <= i_key;
            r_key_sync1 <= r_key_sync0;
        end
    end

// Key Filter FSM
    typedef enum logic [2:0] {
        S_IDLE,
        S_TRIGGER,
        S_STABLE,
        S_RELEASE
    } state_t;

    logic [24:0] cnt_20ms;

    state_t curr_state;

    // event occured in each state
    always_ff @( posedge i_clk, negedge i_rst_n ) begin : alff_FSM
        if (!i_rst_n) begin
            curr_state <= S_IDLE;
            cnt_20ms <= 0;
            o_key_filtered <= 0;
        end
        else begin
            o_key_filtered <= 0;
            case(curr_state)
                S_IDLE: begin
                    if (r_key_sync1 == 0) begin
                        cnt_20ms <= 0;
                        curr_state <= S_TRIGGER;
                    end
                end
                S_TRIGGER: begin
                    if (r_key_sync1 == 1) begin // If shake
                        curr_state <= S_IDLE;
                        cnt_20ms <= 0;
                    end
                    else begin
                        if (cnt_20ms < CNT_20MS_MAX - 1) begin
                            cnt_20ms <= cnt_20ms + 1;
                        end
                        else begin
                            curr_state <= S_STABLE;
                            cnt_20ms <= 0;
                        end
                    end
                end
                S_STABLE : begin
                    if (r_key_sync1 == 1) begin
                        cnt_20ms <= 0;
                        curr_state <= S_RELEASE;
                    end
                end
                S_RELEASE: begin
                    if (r_key_sync1 == 0) begin
                        cnt_20ms <= 0;
                        curr_state <= S_STABLE;
                    end
                    else begin
                        if (cnt_20ms < CNT_20MS_MAX - 1) begin
                            cnt_20ms <= cnt_20ms + 1;
                        end
                        else begin
                            curr_state <= S_IDLE;
                            cnt_20ms <= 0;
                            o_key_filtered <= 1;
                        end
                    end
                end
                default: curr_state <= S_IDLE;
            endcase
        end
    end
    
endmodule
